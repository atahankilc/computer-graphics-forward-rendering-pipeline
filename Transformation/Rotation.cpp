#include "Rotation.h"

Rotation::Rotation() : Base(0) {
    this->angle = 0;
}

Rotation::Rotation(int id, double angle, double x, double y, double z) : Base(id, x, y, z) {
    this->angle = angle;
    Vec3 u = Vec3(x, y, z, Color()), v, w;
    double minComp = std::min(std::min(abs(x), abs(y)), abs(z));
    if (minComp == abs(x))
        v = Vec3(0, -1 * z, y, Color());
    else if (minComp == abs(y))
        v = Vec3(-1 * z, 0, x, Color());
    else if (minComp == abs(z))
        v = Vec3(-1 * y, x, 0, Color());
    w = crossProductVec3(u, v);
    v = normalizeVec3(v);
    w = normalizeVec3(w);
    double translate[4][4] = {{u.x, u.y, u.z, 0},
                              {v.x, v.y, v.z, 0},
                              {w.x, w.y, w.z, 0},
                              {0,   0,   0,   1}};
    double revert[4][4] = {{u.x, v.x, w.x, 0},
                           {u.y, v.y, w.y, 0},
                           {u.z, v.z, w.z, 0},
                           {0,   0,   0,   1}};
    double rotate[4][4] = {{1, 0,                             0,                                    0},
                           {0, cos(this->angle * M_PI / 180), (-1) * sin(this->angle * M_PI / 180), 0},
                           {0, sin(this->angle * M_PI / 180), cos(this->angle * M_PI / 180),        0},
                           {0, 0,                             0,                                    1}};
    this->matrix = multiplyMatrixWithMatrix(revert, multiplyMatrixWithMatrix(rotate, translate));
}

void Rotation::ComputeMatrix() {
    Vec3 u = Vec3(x, y, z, Color()), v, w;
    double minComp = std::min(std::min(abs(x), abs(y)), abs(z));
    if (minComp == abs(x))
        v = Vec3(0, -1 * z, y, Color());
    else if (minComp == abs(y))
        v = Vec3(-1 * z, 0, x, Color());
    else if (minComp == abs(z))
        v = Vec3(-1 * y, x, 0, Color());
    w = crossProductVec3(u, v);
    v = normalizeVec3(v);
    w = normalizeVec3(w);
    double translate[4][4] = {{u.x, u.y, u.z, 0},
                              {v.x, v.y, v.z, 0},
                              {w.x, w.y, w.z, 0},
                              {0,   0,   0,   1}};
    double revert[4][4] = {{u.x, v.x, w.x, 0},
                           {u.y, v.y, w.y, 0},
                           {u.z, v.z, w.z, 0},
                           {0,   0,   0,   1}};
    double rotate[4][4] = {{1, 0,                             0,                                    0},
                           {0, cos(this->angle * M_PI / 180), (-1) * sin(this->angle * M_PI / 180), 0},
                           {0, sin(this->angle * M_PI / 180), cos(this->angle * M_PI / 180),        0},
                           {0, 0,                             0,                                    1}};
    this->matrix = multiplyMatrixWithMatrix(revert, multiplyMatrixWithMatrix(rotate, translate));
}

ostream &operator<<(ostream &os, const Rotation &r) {
    os << fixed << setprecision(3) << "Rotation " << r.id << " => [angle=" << r.angle << ", " << r.x << ", " << r.y
       << ", " << r.z << "]";
    return os;
}
