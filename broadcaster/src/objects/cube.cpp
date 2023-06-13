#include "objects/cube.h"


Cube::Cube(float X, float Y, float Z, float RotationX, float RotationY, float RotationZ, float HalfSizeX, float HalfSizeY, float HalfSizeZ) :
    Object(X, Y, Z, RotationX, RotationY, RotationZ), HalfSizeX(HalfSizeX), HalfSizeY(HalfSizeY), HalfSizeZ(HalfSizeZ)
{
}


ObjectType Cube::GetType()
{
    return ObjectType::ObjectCube;
}
