#include "SegmentLine.h"

#include "Line.h"
#include "RayLine.h"

#include <ostream>

SegmentLine::SegmentLine()
{
}

SegmentLine::SegmentLine(const Point& a, const Point& b)
    : _orig(a)
    , _dest(b)
{
}

SegmentLine::SegmentLine(const SegmentLine& segment)
    : _orig(segment._orig)
    , _dest(segment._dest)
{
}

SegmentLine::SegmentLine(double ax, double ay, double bx, double by)
    : _orig(ax, ay)
    , _dest(bx, by)
{
}

SegmentLine::~SegmentLine()
{
}

Point SegmentLine::getA()
{
    return _orig;
}

Point SegmentLine::getB()
{
    return _dest;
}

SegmentLine& SegmentLine::operator=(const SegmentLine& segment)
{
    if(this != &segment)
    {
        this->_orig = segment._orig;
        this->_dest = segment._dest;
    }

    return *this;
}

bool SegmentLine::segmentIntersection(SegmentLine& l)
{
    Point a = _orig;
    Point b = _dest;
    Point c = l._orig;
    Point d = l._dest;

    if(a.colinear(c, d) || b.colinear(c, d) || c.colinear(a, b) || d.colinear(a, b))
    {
        return false;
    }
    else
    {
        return a.left(c, d) ^ b.left(c, d) && c.left(a, b) ^ d.left(a, b);
    }
}

bool SegmentLine::impSegmentIntersection(SegmentLine& segment)
{
    Point a = _orig;
    Point b = _dest;
    Point c = segment._orig;
    Point d = segment._dest;

    if(a.equal(c) || b.equal(c) || a.equal(d) || b.equal(d))
    {
        return true;
    }

    if(a.colinear(c, d) || b.colinear(c, d) || c.colinear(a, b) || d.colinear(a, b))
    {
        return true;
    }

    return segmentIntersection(segment);
}

void SegmentLine::setA(Point& p)
{
    _orig = p;
}

double SegmentLine::getC()
{
    if(slope() == INFINITY)
    {
        return INFINITY;
    }
    return _orig.getY() - slope() * _orig.getX();
}

bool SegmentLine::distinct(SegmentLine& segment)
{
    return !equal(segment);
}

bool SegmentLine::intersects(Vect2d& c, Vect2d& d, double& s, double& t)
{
    Vect2d* cd = new Vect2d(d.getX() - c.getX(), d.getY() - c.getY());
    Vect2d* ab = new Vect2d(this->getB().getX() - this->getA().getX(), this->getB().getY() - this->getA().getY());
    Vect2d* ac = new Vect2d(c.getX() - this->getA().getX(), c.getY() - this->getA().getY());
    if(BasicGeometry::equal(0.0, cd->getX() * ab->getY() - ab->getX() * cd->getY()))
    {
        return false;
    }
    s = (cd->getX() * ac->getY() - ac->getX() * cd->getY()) / (cd->getX() * ab->getY() - ab->getX() * cd->getY());
    t = (ab->getX() * ac->getY() - ac->getX() * ab->getY()) / (cd->getX() * ab->getY() - ab->getX() * cd->getY());
    return true;
}

bool SegmentLine::intersects(Line& r, Vect2d& res)
{
    double s, t;
    if(this->intersects(dynamic_cast<Vect2d&>(r._orig), dynamic_cast<Vect2d&>(r._dest), s, t))
    {
        if((0 < s && s < 1) || BasicGeometry::equal(0.0, s) || BasicGeometry::equal(1.0, s))
        {
            res = this->getPoint(s);
            return true;
        }
    }
    return false;
}

bool SegmentLine::intersects(RayLine& r, Vect2d& res)
{
    double s, t;
    if(this->intersects(dynamic_cast<Vect2d&>(r._orig), dynamic_cast<Vect2d&>(r._dest), s, t))
    {
        if(((0 < s && s < 1) || BasicGeometry::equal(0.0, s) || BasicGeometry::equal(1.0, s)) && (t > 0 || BasicGeometry::equal(t, 0.0)))
        {
            res = this->getPoint(s);
            return true;
        }
    }
    return false;
}

bool SegmentLine::intersects(SegmentLine& r, Vect2d& res)
{
    double s, t;
    if(this->intersects(dynamic_cast<Vect2d&>(r._orig), dynamic_cast<Vect2d&>(r._dest), s, t))
    {
        if(((0 < s && s < 1) || BasicGeometry::equal(0.0, s) || BasicGeometry::equal(1.0, s)) && ((0 < t && t < 1) || BasicGeometry::equal(0.0, t) || BasicGeometry::equal(1.0, t)))
        {
            res = this->getPoint(s);
            return true;
        }
    }
    return false;
}

double SegmentLine::distPointSegment(Vect2d& vector)
{
    Vect2d*      d = new Vect2d((this->getB() - this->getA()).getX(), (this->getB() - this->getA()).getY());
    const double t = d->dot(*new Vect2d(vector.getX() - this->getA().getX(), vector.getY() - this->getA().getY())) / d->dot(*d);
    double       distance;
    if(t < 0 || BasicGeometry::equal(t, 0.0))
    {
        Vect2d* aux = new Vect2d(vector.getX() - this->getA().getX(), vector.getY() - this->getA().getY());
        distance    = aux->getModule();
    }
    else if(t > 1 || BasicGeometry::equal(t, 1.0))
    {
        Vect2d* aux = new Vect2d(vector.getX() - this->getB().getX(), vector.getY() - this->getB().getY());
        distance    = aux->getModule();
    }
    else
    {
        Vect2d* aux = new Vect2d(vector.getX() - (this->getA().getX() + d->scalarMult(t).getX()), vector.getY() - (this->getA().getY() + d->scalarMult(t).getY()));
        distance    = aux->getModule();
    }

    return distance;
}

float SegmentLine::getDistanceT0(Vect2d& point)
{
    return 0.0f;
}

bool SegmentLine::equal(SegmentLine& segment)
{
    return (_orig.equal(segment._orig) && _dest.equal(segment._dest)) || (_orig.equal(segment._dest) && _dest.equal(segment._orig));
}

Point SegmentLine::getPoint(const double t)
{
    return {this->getA() + (this->getB() - this->getA()).scalarMult(t)};
}

bool SegmentLine::isHorizontal()
{
    if(BasicGeometry::equal(_orig.getY(), _dest.getY()))
    {
        return true;
    }

    return false;
}

bool SegmentLine::isVertical()
{
    if(BasicGeometry::equal(_orig.getX(), _dest.getX()))
    {
        return true;
    }

    return false;
}

bool SegmentLine::isTvalid(double t)
{
    return t >= 0 && t <= 1;
}

bool SegmentLine::left(Point& p)
{
    return p.left(_orig, _dest);
}

double SegmentLine::length()
{
    return _orig.distance(_dest);
}

double SegmentLine::slope()
{
    if(this->isHorizontal())
    {
        return 0.0;
    }

    if(this->isVertical())
    {
        return INFINITY;
    }

    return (_dest.getY() - _orig.getY()) / (_dest.getX() - _orig.getX());
}

double SegmentLine::triangleArea2(Point& p)
{
    return p.triangleArea2(_orig, _dest);
}

std::ostream& operator<<(std::ostream& os, const SegmentLine& segment)
{
    os << "Point A: " << segment._orig << ", Point B: " << segment._dest << "\n";

    return os;
}
