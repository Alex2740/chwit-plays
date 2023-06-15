FROM mongo:latest

ARG DATABASE_URI

COPY ./init-data/data /init-data

CMD mongoimport --jsonArray --uri mongodb://mongodb:27017/openGL --collection scene --file /init-data/init_scene.json && \
    mongoimport --jsonArray --uri mongodb://mongodb:27017/openGL --collection camera --file /init-data/init_camera.json
