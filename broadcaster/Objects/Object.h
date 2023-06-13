#pragma once


enum ObjectType {
	ObjectCube,
};


class Object
{
public:
	static int CompterId;
	int Id;

	float X, Y, Z;
	float RotationX, RotationY, RotationZ;

	Object(float X, float Y, float Z, float RotationX, float RotationY, float RotationZ);

	virtual ObjectType GetType() = 0;
};
