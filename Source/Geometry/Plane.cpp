#include "Plane.h"
#include "BasicGeometry.h"

#include <iostream>

Plane::Plane(Vect3d& p, Vect3d& u, Vect3d& v, bool arePoints)
{
    if(!arePoints)    // Vectors: pi = p + u * lambda + v * mu
    {
        _a = p;
        _b = u.add(_a);
        _c = v.add(_a);
    }
    else
    {    // 3 points in the plane
        _a = p;
        _b = u;
        _c = v;
    }
}

Plane::Plane(const Plane& plane)
    : _a(plane._a)
    , _b(plane._b)
    , _c(plane._c)
{
}

Plane::~Plane()
{
}

bool Plane::coplanar(Vect3d& point)
{
    // XXXX
    return false;
}

double Plane::distance(Vect3d& point)
{
    // XXXX
    return 0;
}

double Plane::getA()
{
    return (BasicGeometry::determinant2x2(_c.getZ() - _a.getZ(), _c.getY() - _a.getY(), _b.getY() - _a.getY(), _b.getZ() - _a.getZ()));
}

double Plane::getB()
{
    return (BasicGeometry::determinant2x2(_c.getX() - _a.getX(), _c.getZ() - _a.getZ(), _b.getZ() - _a.getZ(), _b.getX() - _a.getX()));
}

double Plane::getC()
{
    return (BasicGeometry::determinant2x2(_c.getY() - _a.getY(), _c.getX() - _a.getX(), _b.getX() - _a.getX(), _b.getY() - _a.getY()));
}

Vect3d Plane::getNormal()
{
    // XXXX
    return Vect3d();
}

bool Plane::intersect(Plane& plane, Line3d& line)
{
    // XXXX
    return true;
}

bool Plane::intersect(Line3d& line, Vect3d& point)
{
    // XXXX
    return true;
}

bool Plane::intersect(Plane& pa, Plane& pb, Vect3d& pinter)
{
    // XXXX
    return true;
}

bool intersect(Plane& pa, Plane& pb, Vect3d& pinter)
{
    // XXXX
    return true;
}

Plane& Plane::operator=(const Plane& plane)
{
    if(this != &plane)
    {
        _a = plane._a;
        _b = plane._b;
        _c = plane._c;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Plane& plane)
{
    os << "Plane -> a: " << plane._a << ", b: " << plane._b << ", c: " << plane._c;
    return os;
}