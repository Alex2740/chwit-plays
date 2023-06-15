#include "objects/object.h"


Object::Object(glm::vec3 Position, glm::vec3 Orientation) :
	Position(Position), Orientation(Orientation)
{
	this->Id = CompterId++;
}

void Object::SetPositionOrientation(glm::vec3 Position, glm::vec3 Orientation)
{
	this->Position = Position;
	this->Orientation = Orientation;
}

int Object::CompterId = 0;