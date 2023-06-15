#pragma once

#include <glm/glm.hpp>

enum ObjectType {
	ObjectCube,
};


class Object
{
public:
	static int CompterId;
	int Id;

	glm::vec3 Position;
	glm::vec3 Orientation;

	Object(glm::vec3 Position, glm::vec3 Orientation);

	void SetPositionOrientation(glm::vec3 Position, glm::vec3 Orientation);

	virtual ObjectType GetType() = 0;
};
