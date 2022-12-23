#ifndef __VEC3_H__
#define __VEC3_H__

#include <iostream>
#include "../Color.h"
using namespace std;

class Vec3
{
public:
    double x, y, z;
    Color color;

    Vec3();
    Vec3(double x, double y, double z, const Color& color);
    Vec3(const Vec3 &other);

    double getElementAt(int index);
    
    friend std::ostream& operator<<(std::ostream& os, const Vec3& v);
};

#endif