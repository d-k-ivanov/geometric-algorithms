#include "Line.h"

#include "RayLine.h"
#include "SegmentLine.h"

Line::Line(const Point& a, const Point& b)
    : SegmentLine(a, b)
{
}

Line::Line(const Line& s)
    : SegmentLine(s)
{
}

Line::~Line()
{
}

bool Line::intersects(Line& line, Vect2d& intersection)
{
    double s;
    double t;
    if(SegmentLine::intersects(line._orig, line._dest, s, t))
    {
        intersection = this->getPoint(s);
        return true;
    }
    return false;
}

bool Line::intersects(RayLine& ray, Vect2d& intersection)
{
    double s;
    double t;
    if(SegmentLine::intersects(ray._orig, ray._dest, s, t))
    {
        if((0 < t || BasicGeometry::equal(t, 0.0)))
        {
            intersection = this->getPoint(s);
            return true;
        }
    }
    return false;
}

bool Line::intersects(SegmentLine& segment, Vect2d& intersection)
{
    return segment.intersects(*this, intersection);
}

double Line::distancePointLine(Vect2d& v)
{
    Vect2d*      d   = new Vect2d((this->getB() - this->getA()).getX(), (this->getB() - this->getA()).getY());
    const double t0  = d->dot(*new Vect2d(v.getX() - this->getA().getX(), v.getY() - this->getA().getY())) / d->dot(*d);
    Vect2d*      resultV = new Vect2d(v.getX() - (this->getA().getX() + d->scalarMult(t0).getX()), v.getY() - (this->getA().getY() + d->scalarMult(t0).getY()));
    return resultV->getModule();
}

bool Line::incorrectSegmentIntersection(SegmentLine& l)
{
    return false;
}

Line& Line::operator=(const Line& line)
{
    if(this != &line)
    {
        SegmentLine::operator=(line);
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Line& line)
{
    os << "Line --->\n\tPoint A: " << line._orig << "\n\tPoint B: " << line._dest << "\n";

    return os;
}

bool Line::segmentIntersection(SegmentLine& l)
{
    // XXXX
    return false;
}