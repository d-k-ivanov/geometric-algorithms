#include "Plane.h"
#include "BasicGeometry.h"

#include <iostream>

Plane::Plane(const Vect3d& p, const Vect3d& u, const Vect3d& v, const bool arePoints)
{
    // Vectors: pi = p + u * lambda + v * mu
    // const Vect3d uN    = u.normalize();
    // const Vect3d vN    = v.normalize();
    // const double l     = (uN.dot(vN) * vN.dot(p) - uN.dot(p)) / (1 - (uN.dot(vN)) * (uN.dot(vN)));
    // const double mu    = (uN.dot(vN) * uN.dot(p) - uN.dot(p)) / (1 - (uN.dot(vN)) * (uN.dot(vN)));
    // const double x     = p.getX() + l * uN.getX() + mu * vN.getX();
    // const double y     = p.getY() + l * uN.getY() + mu * vN.getY();
    // const double z     = p.getZ() + l * uN.getZ() + mu * vN.getZ();
    // const Vect3d n     = {x, y, z};
    // const double nlong = n.module();
    // _a = x / nlong;
    // _b = y / nlong;
    // _c = z / nlong;
    // _d = -1.0 * nlong;

    if(!arePoints)
    {    // Vectors: pi = p + u * lambda + v * mu
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
    : _a {plane._a}
    , _b {plane._b}
    , _c {plane._c}
{
}

Plane::~Plane()
{
}

Vect3d& Plane::get_A()
{
    return _a;
}

Vect3d& Plane::get_B()
{
    return _b;
}

Vect3d& Plane::get_C()
{
    return _c;
}

Vect3d& Plane::get_D()
{
    return _d;
}

double Plane::getA() const
{
    return BasicGeometry::determinant2x2(_c.getZ() - _a.getZ(), _c.getY() - _a.getY(), _b.getY() - _a.getY(), _b.getZ() - _a.getZ());
}

double Plane::getB() const
{
    return BasicGeometry::determinant2x2(_c.getX() - _a.getX(), _c.getZ() - _a.getZ(), _b.getZ() - _a.getZ(), _b.getX() - _a.getX());
}

double Plane::getC() const
{
    return BasicGeometry::determinant2x2(_c.getY() - _a.getY(), _c.getX() - _a.getX(), _b.getX() - _a.getX(), _b.getY() - _a.getY());
}

double Plane::getD() const
{
    return -1.0 * (getA() * _a.getX() + getB() * _a.getY() + getC() * _a.getZ());
}

Vect3d Plane::getNormal() const
{
    return {this->getA(), this->getB(), this->getC()};
}

Vect3d Plane::getCenter() const
{
    return _a.add(_b).add(_c).scalarMul(1.0 / 3.0);
}

bool Plane::coplanar(const Vect3d& point) const
{
    if(BasicGeometry::equal(0.0, distance(point)))
    {
        return true;
    }
    return false;
}

// Distance between the plane and the point.
// Given: ax + by + cz + d = 0
// n = ai + bj + ck
// if n is a unit vector, then l = -(n . p + d)
// if n is not a unit vector, then l = -(n . p + d) / (n . n)
// d = |l * n|
double Plane::distance(const Vect3d& point) const
{
    const Vect3d n = this->getNormal();
    double       l = -(n.dot(point) + this->getD());
    if(!BasicGeometry::equal(1.0, n.module()))
    {
        l /= n.dot(n);
    }
    return n.scalarMul(l).module();
}

bool Plane::intersect(const Plane& plane, Line3d& intersectionLine) const
{
    const Vect3d n1        = this->getNormal();
    const Vect3d n2        = plane.getNormal();
    const Vect3d direction = n1.xProduct(n2);
    const double det       = BasicGeometry::determinant3x3(n1.getX(), n1.getY(), n1.getZ(), n2.getX(), n2.getY(), n2.getZ(), direction.getX(), direction.getY(), direction.getZ());
    if(BasicGeometry::equal(0.0, det))
    {
        return false;
    }

    const double d  = plane.getD();
    const double x0 = (d * BasicGeometry::determinant2x2(n1.getY(), n1.getZ(), direction.getY(), direction.getZ()) - d * BasicGeometry::determinant2x2(n2.getY(), n2.getZ(), direction.getY(), direction.getZ())) / det;
    const double y0 = (d * BasicGeometry::determinant2x2(direction.getX(), direction.getZ(), n1.getX(), n1.getZ()) - d * BasicGeometry::determinant2x2(direction.getX(), direction.getZ(), n2.getX(), n2.getZ())) / det;
    const double z0 = (d * BasicGeometry::determinant2x2(n1.getX(), n1.getY(), direction.getX(), direction.getY()) - d * BasicGeometry::determinant2x2(n2.getX(), n2.getY(), direction.getX(), direction.getY())) / det;

    Vect3d p0(x0, y0, z0);
    Vect3d p1(x0 + direction.getX(), y0 + direction.getY(), z0 + direction.getZ());
    intersectionLine = {p0, p1};

    return true;
}

bool Plane::intersect(Line3d& line, Vect3d& intersectionPoint) const
{
    const Vect3d origin = line.getOrigin();
    const Vect3d dest   = line.getDestination();
    const Vect3d v      = dest.sub(origin);
    const Vect3d n      = this->getNormal();

    if(BasicGeometry::equal(0.0, n.dot(v)))
    {
        return false;
    }

    const double d = this->getD();
    double       l = -(n.dot(origin) + d);
    if(!BasicGeometry::equal(1.0, v.module()))
    {
        l /= n.dot(v);
    }

    intersectionPoint = origin.add(v.scalarMul(l));
    return true;
}

bool Plane::intersect(const Plane& planeA, const Plane& planeB, Vect3d& intersectionPoint) const
{
    const Vect3d n1 = planeA.getNormal();
    const Vect3d n2 = planeB.getNormal();
    const Vect3d n3 = this->getNormal();

    const double det = BasicGeometry::determinant3x3(n1.getX(), n1.getY(), n1.getZ(), n2.getX(), n2.getY(), n2.getZ(), n3.getX(), n3.getY(), n3.getZ());
    if(BasicGeometry::equal(0.0, det))
    {
        return false;
    }

    const double d1 = planeA.getD();
    const double d2 = planeB.getD();
    const double d3 = this->getD();

    const double x = BasicGeometry::determinant3x3(d1, n1.getY(), n1.getZ(), d2, n2.getY(), n2.getZ(), d3, n3.getY(), n3.getZ()) / det;
    const double y = BasicGeometry::determinant3x3(n1.getX(), d1, n1.getZ(), n2.getX(), d2, n2.getZ(), n3.getX(), d3, n3.getZ()) / det;
    const double z = BasicGeometry::determinant3x3(n1.getX(), n1.getY(), d1, n2.getX(), n2.getY(), d2, n3.getX(), n3.getY(), d3) / det;

    intersectionPoint = {x, y, z};
    return true;
}

// Returns the reflected point of p in the plane.
// n = normal vector of the plane
// if n is a unit vector, then l = -2 * (n . p + d)
// if n is not a unit vector, then l = -2 * (n . p + d) / (n . n)
// q = p + l * n
Vect3d Plane::reflectedPoint(const Vect3d& p) const
{
    const Vect3d n = this->getNormal();
    double       l = -2 * (n.dot(p) + this->getD());
    if(!BasicGeometry::equal(1.0, n.module()))
    {
        l /= n.dot(n);
    }
    return p.add(n.scalarMul(l));
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
