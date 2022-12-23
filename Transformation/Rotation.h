#ifndef __ROTATION_H__
#define __ROTATION_H__

#include <math.h>
#include "Base.h"

class Rotation : public Base {
public:
    double angle;

    Rotation();
    Rotation(int, double, double, double, double);
    void ComputeMatrix() override;

    friend std::ostream &operator<<(std::ostream&, const Rotation&);
};

#endif