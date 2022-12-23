#include "Scaling.h"

Scaling::Scaling() : Base(1) {
}

Scaling::Scaling(int id, double x, double y, double z) : Base(id, x, y, z) {
    double val[4][4] = {{x, 0, 0, 0},
                        {0, y, 0, 0},
                        {0, 0, z, 0},
                        {0, 0, 0, 1}};
    this->matrix = Matrix4(val);
}

void Scaling::ComputeMatrix() {
    double val[4][4] = {{this->x, 0, 0, 0},
                        {0, this->y, 0, 0},
                        {0, 0, this->z, 0},
                        {0, 0, 0, 1}};
    this->matrix = Matrix4(val);
}
