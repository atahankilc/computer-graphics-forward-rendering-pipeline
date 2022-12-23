#ifndef __TRANSLATION_H__
#define __TRANSLATION_H__

#include "Base.h"

class Translation : public Base {
public:
    Translation();
    Translation(int, double, double, double);
    void ComputeMatrix() override;
};

#endif