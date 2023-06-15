#include "objects/cube.h"


Cube::Cube(glm::vec3 Position, glm::vec3 Orientation, glm::vec3 HalfSize) :
    Object(Position, Orientation), HalfSize(HalfSize)
{
}


ObjectType Cube::GetType()
{
    return ObjectType::ObjectCube;
}
