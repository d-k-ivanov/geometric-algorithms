#include "Vect2d.h"

namespace GDSA::Geometry
{

Vect2d::Vect2d(const double x, const double y)
    : Point(x, y)
{
}

Vect2d::Vect2d(const Point& point)
    : Point(point)
{
}

Vect2d::Vect2d(const Vect2d& vector)
    : Point(vector._x, vector._y)
{
}

double Vect2d::getX()
{
    return _x;
}

double Vect2d::getY()
{
    return _y;
}

double Vect2d::dot(const Vect2d& b) const
{
    return _x * b._x + _y * b._y;
}

Vect2d Vect2d::operator+(const Vect2d& b)
{
    _x += b._x;
    _y += b._y;

    return *this;
}

Vect2d Vect2d::operator+=(const Vect2d& b)
{
    _x += b._x;
    _y += b._y;

    return *this;
}

Vect2d Vect2d::operator-(const Vect2d& b)
{
    _x -= b._x;
    _y -= b._y;

    return *this;
}

Vect2d Vect2d::ScalarMult(const double t)
{
    _x *= t;
    _y *= t;

    return *this;
}
}    // namespace GDSA::Geometry
