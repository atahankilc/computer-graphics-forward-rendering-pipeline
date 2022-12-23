#include "Translation.h"

Translation::Translation() : Base(0) {
}

Translation::Translation(int id, double x, double y, double z) : Base(id, x, y, z) {
    double val[4][4] = {{1, 0, 0, x},
                        {0, 1, 0, y},
                        {0, 0, 1, z},
                        {0, 0, 0, 1}};
    this->matrix = Matrix4(val);
}

void Translation::ComputeMatrix() {
    double val[4][4] = {{1, 0, 0, this->x},
                       {0, 1, 0, this->y},
                       {0, 0, 1, this->z},
                       {0, 0, 0, 1}};
    this->matrix = Matrix4(val);
}


