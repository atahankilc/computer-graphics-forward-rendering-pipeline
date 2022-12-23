
#include "Vec4.h"
#include <iomanip>

using namespace std;

Vec4::Vec4()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->t = 0.0;
    this->color = Color();
}

Vec4::Vec4(double x, double y, double z, double t, const Color& color)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->t = t;
    this->color = Color(color);
}

Vec4::Vec4(const Vec3 &other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->t = 1;
    this->color = Color(other.color);
}

Vec4::Vec4(const Vec4 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->t = other.t;
    this->color = Color(other.color);
}

double Vec4::getElementAt(int index)
{
    switch (index)
    {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
        return this->z;

    case 3:
        return this->t;

    default:
        return this->t;
    }
}

Vec4 Vec4::operator/(double w) const {
    Vec4 lhs;
    lhs.x = this->x/w;
    lhs.y = this->y/w;
    lhs.z = this->z/w;
    lhs.t = 1;
    lhs.color = Color(this->color);
    return lhs;
}

ostream& operator<<(ostream& os, const Vec4& v) {
    
    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.t << "]";

    return os;
}
