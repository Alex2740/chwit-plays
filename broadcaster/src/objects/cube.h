#pragma once

#include "objects/object.h"

#include <array>


class Cube : public Object
{
public:
	Cube(glm::vec3 Position, glm::vec3 Orientation, glm::vec3 HalfSize);

	glm::vec3 HalfSize;

	ObjectType GetType() override;
};
