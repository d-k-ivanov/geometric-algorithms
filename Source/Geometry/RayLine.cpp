#include "RayLine.h"

#include "Line.h"

#include <ostream>

namespace GDSA::Geometry
{
RayLine::RayLine(const Point& a, const Point& b)
    : SegmentLine(a, b)
{
}

RayLine& RayLine::operator=(const RayLine& rayline)
{
    if(this != &rayline)
    {
        SegmentLine::operator=(rayline);
    }

    return *this;
}

bool RayLine::incorrectSegmentIntersection(SegmentLine& segment) const
{
    return false;
}

bool RayLine::intersects(Line& line, Vect2d& intersection)
{
    return line.intersects(*this, intersection);
}

bool RayLine::intersects(RayLine& rayline, Vect2d& intersection)
{
    double s;
    double t;
    if(SegmentLine::intersects(rayline._orig, rayline._dest, s, t))
    {
        if((0 < s || BasicGeometry::equal(s, 0.0)) && (0 < t || BasicGeometry::equal(t, 0.0)))
        {
            intersection = this->getPoint(s);
            return true;
        }
    }
    return false;
}

bool RayLine::intersects(SegmentLine& segment, Vect2d& intersection)
{
    return segment.intersects(*this, intersection);
}

double RayLine::distanceToPoint(Vect2d& vector)
{
    auto*      d  = new Vect2d((this->getB() - this->getA()).getX(), (this->getB() - this->getA()).getY());
    const auto t0 = d->dot(*new Vect2d(vector.getX() - this->getA().getX(), vector.getY() - this->getA().getY())) / d->dot(*d);

    double distance;
    if(t0 < 0 || BasicGeometry::equal(t0, 0.0))
    {
        const auto* resultV = new Vect2d(vector.getX() - this->getA().getX(), vector.getY() - this->getA().getY());
        distance            = resultV->getModule();
    }
    else
    {
        const auto* resultV = new Vect2d(vector.getX() - (this->getA().getX() + d->ScalarMult(t0).getX()), vector.getY() - (this->getA().getY() + d->ScalarMult(t0).getY()));
        distance            = resultV->getModule();
    }

    return distance;
}

std::ostream& operator<<(std::ostream& os, const RayLine& ray)
{
    os << "Point A: " << ray._orig << ", Point B: " << ray._dest << "\n";

    return os;
}
}    // namespace GDSA::Geometry
