#include "Line3d.h"

#include "BasicGeometry.h"

#include "Utils/RandomUtilities.h"

#include <iostream>

namespace GDSA::Geometry
{
Line3d::Line3d(Vect3d& orig, Vect3d& dest)
    : Edge3d(orig, dest)
{
}

bool Line3d::isTvalid(double t)
{
    return true;
}

// Distance between two lines.
// p1 = q1 + v1 * t
// p2 = q2 + v2 * t
// d = |(q1 - q2) . (v1 x v2)| / |v1 x v2|
double Line3d::distance(Line3d& line)
{
    constexpr double deltaT = 10.0;
    const Vect3d*    q1     = new Vect3d(this->getOrigin());
    const Vect3d*    v1     = new Vect3d(this->getPoint(deltaT));
    const Vect3d*    q2     = new Vect3d(line.getOrigin());
    const Vect3d*    v2     = new Vect3d(line.getPoint(deltaT));
    const Vect3d     vxv    = v1->xProduct(*v2);

    return (q1->sub(*q2).dot(vxv)) / vxv.module();
}

// Distance between the line and the point.
// q = t + v * l
// if v is a unit vector, then l = v . (p - t)
// if v is not a unit vector, then l = (v . (p - t)) / (v . v)
// d = |p - t - (v * l)|
double Line3d::distance(const Vect3d& p) const
{
    const Vect3d q = this->_dest;
    const Vect3d t = this->_orig;
    const Vect3d v = q.sub(t);

    double l = v.dot(p.sub(t));
    if(!BasicGeometry::equal(1.0, v.module()))
    {
        l /= v.dot(v);
    }

    // const Vect3d pq = p - t - v * l;
    const Vect3d pq = p.sub(t).sub(v.scalarMul(l));
    return pq.module();
}

// Normal line to the line that passes through p.
// q = t + v * l
// if v is a unit vector, then l = v . (p - t)
// if v is not a unit vector, then l = (v . (p - t)) / (v . v)
// u = p - (t + (v * l))
Line3d Line3d::normalLine(Vect3d& p) const
{
    const Vect3d q = this->_dest;
    const Vect3d t = this->_orig;
    const Vect3d v = q.sub(t);

    double l = v.dot(p.sub(t));
    if(!BasicGeometry::equal(1.0, v.module()))
    {
        l /= v.dot(v);
    }

    // Vect3d u = p.sub(t.add(v.scalarMul(l)));
    Vect3d u = t.add(v.scalarMul(l));
    return {u, p};
}

// Check if two lines are parallel.
// p1 = q1 + v1 * t
// p2 = q2 + v2 * t
bool Line3d::isParallel(Line3d& line)
{
    const Vect3d* v1 = new Vect3d(this->getOrigin().sub(this->getDestination()));
    const Vect3d* v2 = new Vect3d(line.getOrigin().sub(line.getDestination()));
    if(BasicGeometry::equal(0.0, v1->xProduct(*v2).module()))
    {
        return true;
    }
    return false;
}

// Check if two lines are perpendicular.
// v1 = p1 - p2
// v2 = p3 - p4
// v1 . v2 = 0
bool Line3d::isPerpendicular(Line3d& line)
{
    const Vect3d* v1 = new Vect3d(this->getOrigin().sub(this->getDestination()));
    const Vect3d* v2 = new Vect3d(line.getOrigin().sub(line.getDestination()));
    if(BasicGeometry::equal(0.0, v1->dot(*v2)))
    {
        return true;
    }
    return false;
}

Line3d& Line3d::operator=(const Line3d& line)
{
    if(this != &line)
    {
        Edge3d::operator=(line);
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Line3d& line)
{
    std::cout << "Line -> Origin: " << line._orig << ", Destination: " << line._dest;
    return os;
}
}