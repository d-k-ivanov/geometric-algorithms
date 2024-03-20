#include "SegmentLine.h"

#include "Line.h"
#include "RayLine.h"

#include <ostream>

namespace GDSA::Geometry
{
SegmentLine::SegmentLine(const Point& a, const Point& b)
    : _orig(a)
    , _dest(b)
{
}

SegmentLine::SegmentLine(double ax, double ay, double bx, double by)
    : _orig(ax, ay)
    , _dest(bx, by)
{
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

Point SegmentLine::getA()
{
    return _orig;
}

Point SegmentLine::getB()
{
    return _dest;
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
    if(BasicGeometry::equal(slope(), HUGE_VAL))
    {
        return HUGE_VAL;
    }
    return _orig.getY() - slope() * _orig.getX();
}

bool SegmentLine::distinct(SegmentLine& segment)
{
    return !equal(segment);
}

bool SegmentLine::intersects(Point& c, Point& d, double& s, double& t)
{
    Vect2d* cd = new Vect2d(d.getX() - c.getX(), d.getY() - c.getY());
    Vect2d* ab = new Vect2d(this->getB().getX() - this->getA().getX(), this->getB().getY() - this->getA().getY());
    Vect2d* ac = new Vect2d(c.getX() - this->getA().getX(), c.getY() - this->getA().getY());

    // Check if the lines are parallel and can't intersect
    if(BasicGeometry::equal(0.0, cd->getX() * ab->getY() - ab->getX() * cd->getY()))
    {
        return false;
    }

    // Compute parameters of the intersection point s and t
    s = (cd->getX() * ac->getY() - ac->getX() * cd->getY()) / (cd->getX() * ab->getY() - ab->getX() * cd->getY());
    t = (ab->getX() * ac->getY() - ac->getX() * ab->getY()) / (cd->getX() * ab->getY() - ab->getX() * cd->getY());
    return true;
}

bool SegmentLine::intersects(Line& line, Vect2d& res)
{
    double s, t;
    if(this->intersects(line._orig, line._dest, s, t))
    {
        // s in [0,1] and t any value
        if((0 < s && s < 1) || BasicGeometry::equal(0.0, s) || BasicGeometry::equal(1.0, s))
        {
            res = this->getPoint(s);
            return true;
        }
    }
    return false;
}

bool SegmentLine::intersects(RayLine& ray, Vect2d& res)
{
    double s, t;
    if(this->intersects(ray._orig, ray._dest, s, t))
    {
        // s in [0,1] and t greater or equal than 0
        if(((0 < s && s < 1) || BasicGeometry::equal(0.0, s) || BasicGeometry::equal(1.0, s)) && (t > 0 || BasicGeometry::equal(t, 0.0)))
        {
            res = this->getPoint(s);
            return true;
        }
    }
    return false;
}

bool SegmentLine::intersects(SegmentLine& segment, Vect2d& res)
{
    double s, t;
    if(this->intersects(segment._orig, segment._dest, s, t))
    {
        // s and t in [0,1]
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
    auto*      d  = new Vect2d((this->getB() - this->getA()).getX(), (this->getB() - this->getA()).getY());
    const auto t0 = d->dot(*new Vect2d(vector.getX() - this->getA().getX(), vector.getY() - this->getA().getY())) / d->dot(*d);
    double     distance;

    if(t0 < 0 || BasicGeometry::equal(t0, 0.0))    // A - P
    {
        const auto* resultV = new Vect2d(vector.getX() - this->getA().getX(), vector.getY() - this->getA().getY());
        distance            = resultV->getModule();
    }
    else if(t0 > 1 || BasicGeometry::equal(t0, 1.0))    // A - (P + d)
    {
        const auto* resultV = new Vect2d(vector.getX() - this->getB().getX(), vector.getY() - this->getB().getY());
        distance            = resultV->getModule();
    }
    else    // A - (P + t * d)
    {
        const auto* resultV = new Vect2d(vector.getX() - (this->getA().getX() + d->ScalarMult(t0).getX()), vector.getY() - (this->getA().getY() + d->ScalarMult(t0).getY()));
        distance            = resultV->getModule();
    }

    return distance;
}

double SegmentLine::getDistanceT0(Vect2d& point) const
{
    return 0.0f;
}

bool SegmentLine::equal(SegmentLine& segment) const
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

bool SegmentLine::left(const Point& p)
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
        return HUGE_VAL;
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
}    // namespace GDSA::Geometry
