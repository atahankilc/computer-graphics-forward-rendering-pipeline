#include "Vec3.h"
#include <iomanip>

using namespace std;

Vec3::Vec3() {
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->color = Color();
}

Vec3::Vec3(double x, double y, double z, const Color& color) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->color = Color(color);
}

Vec3::Vec3(const Vec3 &other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->color = Color(other.color);
}

double Vec3::getElementAt(int index) {
    switch (index) {
        case 0:
            return this->x;

        case 1:
            return this->y;

        case 2:
            return this->z;

        default:
            return this->z;
    }
}

ostream &operator<<(ostream &os, const Vec3 &v) {

    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << "]";

    return os;
}
