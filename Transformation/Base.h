#ifndef INC_477_HW2_BASE_H
#define INC_477_HW2_BASE_H

#include <iostream>
#include <iomanip>
#include "../Vector/Vec3.h"
#include "../Vector/Helpers.h"
#include "../Matrix4.h"

class Base {
public:
    int id;
    double x, y, z;
    Matrix4 matrix;

    explicit Base(double);
    Base(int, double, double, double);
    virtual void ComputeMatrix() = 0;
    friend std::ostream &operator<<(std::ostream&, const Base&);
};

#endif //INC_477_HW2_BASE_H