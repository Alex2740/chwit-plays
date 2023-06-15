#!/bin/bash
echo "########### Loading data to Mongo DB ###########"
mongoimport --jsonArray --db openGL --collection scene --file @/test/init_scene.json
mongoimport --jsonArray --db openGL --collection camera --file @/test/init_camera.json