#include "Vect2d.h"

namespace GDSA::Geometry
{
Vect2d::Vect2d()
    : Point()
{
}

Vect2d::Vect2d(double x, double y)
    : Point(x, y)
{
}

Vect2d::Vect2d(const Point& point)
    : Point(point)
{
}

Vect2d::Vect2d(const Vect2d& v)
    : Point(v._x, v._y)
{
}

Vect2d::~Vect2d()
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

double Vect2d::dot(Vect2d& b)
{
    return _x * b._x + _y * b._y;
}

Vect2d& Vect2d::operator=(const Vect2d& vector)
{
    Point::operator=(vector);

    return *this;
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

Vect2d Vect2d::scalarMult(double t)
{
    _x *= t;
    _y *= t;

    return *this;
}
} // namespace GDSA::Geometry
