#include "objects/object.h"


Object::Object(float X, float Y, float Z, float RotationX, float RotationY, float RotationZ) :
	X(X), Y(Y), Z(Z), RotationX(0), RotationY(0), RotationZ(0)
{
	this->Id = CompterId++;
}

int Object::CompterId = 0;