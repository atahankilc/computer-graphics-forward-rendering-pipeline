#ifndef __VEC4_H__
#define __VEC4_H__

#include <iostream>
#include "Vec3.h"
#include "../Color.h"

using namespace std;

class Vec4
{
public:
    double x, y, z, t;
    Color color;


    Vec4();
    Vec4(double x, double y, double z, double t, const Color& color);
    Vec4(const Vec3 &other);
    Vec4(const Vec4 &other);
    
    double getElementAt(int index);

    Vec4 operator/(double w) const;

    friend std::ostream& operator<<(std::ostream& os, const Vec4& v);
};

#endif