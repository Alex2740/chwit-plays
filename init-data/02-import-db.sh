#!/bin/bash

echo "########### Loading data to Mongo DB ###########"
mongoimport --jsonArray --db openGL --collection scene --file /tmp/data/init_scene.json
mongoimport --jsonArray --db openGL --collection camera --file /tmp/data/init_camera.json
