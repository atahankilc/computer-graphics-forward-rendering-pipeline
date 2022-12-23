#ifndef __SCALING_H__
#define __SCALING_H__

#include "Base.h"

class Scaling : public Base {
public:
    Scaling();
    Scaling(int, double, double, double);
    void ComputeMatrix() override;
};

#endif