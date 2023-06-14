#!/bin/bash
echo "########### Loading data to Mongo DB ###########"
mongoimport --jsonArray --db openGL --collection scene --file ./data/init_scene.json
mongoimport --jsonArray --db openGL --collection camera --file ./data/init_camera.json