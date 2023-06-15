from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

from fastapi import FastAPI, status, Request
from fastapi.openapi.utils import get_openapi
from datetime import datetime, timedelta
import uvicorn
import os
from pymongo import mongo_client
import json
from bson import ObjectId, json_util
from load_dotenv import  load_dotenv


load_dotenv()
# Connexion à la base InfluxDB


org = os.environ.get("org")
bucket = os.environ.get("bucket")
url = os.environ.get("influx_url")
usr = os.environ.get("influx_usr")
pwd = os.environ.get("influx_pwd")

app = FastAPI()
client = InfluxDBClient(url=url,username=usr, password=pwd,ssl=True,verify_ssl=True,org=org)
write_api = client.write_api(write_options=SYNCHRONOUS)

#Connexion à la base MongoDB

database = os.environ.get("mongo_database")
collection = os.environ.get("mongo_camera_collection")
mongo_url =  os.environ.get("mongo_url")

mongo_client = mongo_client.MongoClient(mongo_url)

db = mongo_client[database]
cameras_collec = db[collection]


# Fonction de conversion personnalisée pour les objets ObjectId
def json_converter(obj):
    if isinstance(obj, ObjectId):
        return str(obj)  # Convertit ObjectId en chaîne de caractères
    raise TypeError("Type not serializable")

def move_camera(position, input : int):
    x = position['x']
    y = position['y']
    z = position['z']

    if input == 0:
        y += 0.5
    elif input == 1:
        x += 0.5
    elif input == 2:
        y -= 0.5
    elif input == 3:
        x -= 0.5
    elif input == 4:
        z += 0.5
    elif input == 5:
        z -= 0.5

    return {'x' : x, 'y' : y, 'z' : z}

def query_data(query : str):
    query_api = client.query_api()
    result = query_api.query(org=org, query=query)
    results = []
    for table in result:
        for record in table.records:
            results.append((record.get_value(), record.get_field(), record.get_time()))

    return results

def delete_date(measurement, date):
    delete_api = client.delete_api()
    start = "1970-01-01T00:00:00Z"
    stop = date
    delete_api.delete(start, stop, f'_measurement="{measurement}"', bucket=bucket, org=org)


@app.get("/scene/{cameraID}")
def get_scene(cameraID : int):

    cameras = []
    for c in cameras_collec.find():
        cameras.append(c)
    doc_camera = list(cameras)
    doc_scene = list(db["scene"].find())

    query = f'from(bucket: "Inputs")\
    |> range(start: -10d)\
    |> filter(fn:(r) => r._measurement == "Inputs")\
    |> filter(fn:(r) => r.Camera == "{cameraID}")\
    |> filter(fn:(r) => r._field == "value")'


    results = query_data(query)
    result = None

    # S'il n'y a pas d'input à traiter, on renvoie la scène sans y toucher
    if(len(results) == 0):
        data = {"Cameras" : doc_camera, "Scene" : doc_scene}
        return json.loads(json_util.dumps(data))

    input = results[0]

    cam = cameras[cameraID]

    filter = {"_id": cam['_id']}

    newPosition = move_camera(cam['Position'], input[0])
    newValues = {"$set": {'Position': newPosition}}
    cameras_collec.update_one(filter, newValues)

    delete_date("Inputs", (input[2] + timedelta(seconds=1)).strftime("%Y-%m-%dT%H:%M:%SZ"))

    data = {"Cameras": doc_camera, "Scene": doc_scene}
    return json.loads(json_util.dumps(data))

@app.get("/inputs/{cameraID}/{value}", tags=["input"])
def new_input_field(cameraID : int, value: str, request: Request):
    """

    :param cameraID: ID de la caméra qui va être bougée
    :param value: valeur de l'input :
            - 0 : up
            - 1 : right
            - 2 : down
            - 3 : left
            - 4 : forward
            - 5 : backward
    :return: Input
    """
    data = [
        Point("Inputs")
        .tag("Camera",cameraID)
        .field("value",int(float(value)) % 6)
    ]
    write_api.write(bucket, org, data)
    return {"Camera": {cameraID}, "Input" : int(float(value)) %6}

