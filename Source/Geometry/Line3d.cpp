#include "Line3d.h"

#include "BasicGeometry.h"

#include "Utils/RandomUtilities.h"

#include <iostream>

Line3d::Line3d()
{
}

Line3d::Line3d(Vect3d& orig, Vect3d& dest)
    : Edge3d(orig, dest)
{
}

Line3d::Line3d(const Line3d& line)
    : Edge3d(line)
{
}

Line3d::~Line3d()
{
}

double Line3d::distance(Line3d& line)
{
    double  deltaT = 10.0;
    Vect3d* q1     = new Vect3d(this->getOrigin());
    Vect3d* v1     = new Vect3d(this->getPoint(deltaT));
    Vect3d* q2     = new Vect3d(line.getOrigin());
    Vect3d* v2     = new Vect3d(line.getPoint(deltaT));
    Vect3d  vxv    = v1->xProduct(*v2);

    return (q1->sub(*q2).dot(vxv)) / vxv.module();
}

double Line3d::distance(Vect3d& p)
{
    double l = 0.0;
    Vect3d q = this->_dest;
    Vect3d t = this->_orig;
    Vect3d v = q - t;

    if(v.module() == 1)
    {
        l = v.dot(p - t);
    }
    else
    {
        l = (v.dot(p - t)) / (v.dot(v));
    }

    Vect3d pq = (p - t - (v * l));
    return pq.module();
}

Line3d Line3d::normalLine(Vect3d& p)
{
    double l = 0.0;
    Vect3d q = this->_dest;
    Vect3d t = this->_orig;
    Vect3d v = q - t;

    if(v.module() == 1)
    {
        l = v.dot(p - t);
    }
    else
    {
        l = (v.dot(p - t)) / (v.dot(v));
    }

    Vect3d u = p - (t + (v * l));
    return {p, u};
}

bool Line3d::isParallel(Line3d& line)
{
    // Vect3d p1 = this->getPoint(RandomUtilities::getUniformRandom(-100, 100));
    // Vect3d p2 = line.getPoint(RandomUtilities::getUniformRandom(-100, 100));

    double thisDX = this->getDestination()._x - this->getOrigin()._x;
    double thisDY = this->getDestination()._y - this->getOrigin()._y;
    double thisDZ = this->getDestination()._z - this->getOrigin()._z;

    double lineDX = line.getDestination()._x - line.getOrigin()._x;
    double lineDY = line.getDestination()._y - line.getOrigin()._y;
    double lineDZ = line.getDestination()._z - line.getOrigin()._z;

    // if(BasicGeometry::equal(p1.getX() / p2.getX(), p1.getY() / p2.getY()) && BasicGeometry::equal(p1.getZ() / p2.getZ(), p1.getY() / p2.getY()))
    if(thisDX / lineDX == thisDY / lineDY && thisDY / lineDY == thisDZ / lineDZ)
    {
        return true;
    }
    return false;
}

bool Line3d::isPerpendicular(Line3d& line)
{
    Vect3d* v1 = new Vect3d(this->getOrigin().sub(this->getDestination()));
    Vect3d* v2 = new Vect3d(line.getOrigin().sub(line.getDestination()));
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
