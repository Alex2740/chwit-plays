#pragma once

#include "objects/object.h"

#include <array>


class Cube : public Object
{
public:
	Cube(float X, float Y, float Z, float RotationX, float RotationY, float RotationZ, float HalfSizeX, float HalfSizeY, float HalfSizeZ);

	float HalfSizeX, HalfSizeY, HalfSizeZ;

	ObjectType GetType() override;
};
