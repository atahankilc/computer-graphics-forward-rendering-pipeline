#include "Triangle.h"

Triangle::Triangle() {
    this->vertex[0] = nullptr;
    this->vertex[1] = nullptr;
    this->vertex[2] = nullptr;
}

Triangle::Triangle(Vec3 *v1, Vec3 *v2, Vec3 *v3)
{
    this->vertex[0] = v1;
    this->vertex[1] = v2;
    this->vertex[2] = v3;
}

Triangle::Triangle(const Triangle &other)
{
    this->vertex[0] = other.vertex[0];
    this->vertex[1] = other.vertex[1];
    this->vertex[2] = other.vertex[2];
}

// getters
Vec3* Triangle::getFirstVertex()
{
    return this->vertex[0];
}

Vec3* Triangle::getSecondVertex()
{
    return this->vertex[1];
}

Vec3* Triangle::getThirdVertex()
{
    return this->vertex[2];
}
