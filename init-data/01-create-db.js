db = db.getSiblingDB('openGL');
db.createCollection("scene", { capped: false });
db.createCollection("camera", { capped: false });