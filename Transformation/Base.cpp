#include "Base.h"

Base::Base(double placeholder) {
    this->id = -1;
    this->x = placeholder;
    this->y = placeholder;
    this->z = placeholder;
}

Base::Base(int id, double x, double y, double z)
{
    this->id = id;
    this->x = x;
    this->y = y;
    this->z = z;
}

std::ostream &operator<<(std::ostream &os, const Base &t)
{
    os << std::fixed << std::setprecision(3) << "Scaling " << t.id << " => [" << t.x << ", " << t.y << ", " << t.z << "]";

    return os;
}