#include "SegmentLine.h"
#include "StdAfx.h"

#include "Line.h"
#include "RayLine.h"

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

double SegmentLine::getC()
{
    if(slope() == INFINITY)
    {
        return INFINITY;
    }
    return _orig.getY() - (slope() * _orig.getX());
}

bool SegmentLine::distinct(SegmentLine& segment)
{
    return !equal(segment);
}

float SegmentLine::distPointSegment(Vect2d& vector)
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

std::ostream& operator<<(std::ostream& os, const SegmentLine& segment)
{
    os << "Point A: " << segment._orig << ", Point B: " << segment._dest << "\n";

    return os;
}

float SegmentLine::getDistanceT0(Vect2d& point)
{
    return 0.0f;
}