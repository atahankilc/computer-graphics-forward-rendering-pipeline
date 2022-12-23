#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Vector/Vec3.h"

class Triangle
{
public:
    Vec3* vertex[3];

    Triangle();
    Triangle(Vec3 *v1, Vec3 *v2, Vec3 *v3);
    Triangle(const Triangle &other);

    Vec3* getFirstVertex();
    Vec3* getSecondVertex();
    Vec3* getThirdVertex();
};


#endif