#include "Triangle3d.h"

#include "BasicGeometry.h"
#include "Edge3d.h"

#include <iostream>

Triangle3d::Triangle3d()
{
}

Triangle3d::Triangle3d(double ax, double ay, double az, double bx, double by, double bz, double cx, double cy, double cz)
    : _a(ax, ay, az)
    , _b(bx, by, bz)
    , _c(cx, cy, cz)
{
}

Triangle3d::Triangle3d(const Triangle3d& triangle)
    : _a(triangle._a)
    , _b(triangle._b)
    , _c(triangle._c)
{
}

Triangle3d::Triangle3d(Vect3d& va, Vect3d& vb, Vect3d& vc)
    : _a(va)
    , _b(vb)
    , _c(vc)
{
}

Triangle3d::~Triangle3d()
{
}

Vect3d Triangle3d::getA()
{
    return _a;
}

Vect3d Triangle3d::getB()
{
    return _b;
}

Vect3d Triangle3d::getC()
{
    return _c;
}

void Triangle3d::setA(const Vect3d& pa)
{
    _a = pa;
}

void Triangle3d::setB(const Vect3d& pb)
{
    _b = pb;
}

void Triangle3d::setC(const Vect3d& pc)
{
    _c = pc;
}

void Triangle3d::set(const Vect3d& va, const Vect3d& vb, const Vect3d& vc)
{
    _a = va;
    _b = vb;
    _c = vc;
}

double Triangle3d::area() const
{
    const Vect3d ab(_b.sub(_a));
    const Vect3d ac(_c.sub(_a));
    const Vect3d abxac(ab.xProduct(ac));

    return abxac.module() / 2;
}

Triangle3d::PointPosition Triangle3d::classify(const Vect3d& point)
{
    const Vect3d* v   = new Vect3d(point.sub(this->getA()));
    const double  len = v->module();

    if(BasicGeometry::equal(0.0, len))
    {
        return PointPosition::COPLANAR;
    }

    v = new Vect3d(v->scalarMul(1 / len));

    const double d = v->dot(this->normal());
    if(BasicGeometry::equal(0.0, d))
    {
        return PointPosition::COPLANAR;
    }

    if(d < 0.0)
    {
        return PointPosition::NEGATIVE;
    }

    return PointPosition::POSITIVE;
}

Triangle3d Triangle3d::copy()
{
    return Triangle3d(_a, _b, _c);
}

Vect3d Triangle3d::normal() const
{
    const Vect3d v1(_b.sub(_a));
    const Vect3d v2(_c.sub(_a));
    const Vect3d n(v1.xProduct(v2));
    const double module = n.module();

    return n.scalarMul(1.0f / module);
}

Triangle3d& Triangle3d::operator=(const Triangle3d& triangle)
{
    _a = triangle._a;
    _b = triangle._b;
    _c = triangle._c;

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Triangle3d& triangle)
{
    std::cout << "Triangle 3D -> " << triangle._a << ", " << triangle._b << ", " << triangle._c;
    return os;
}
