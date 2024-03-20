#include "Circle.h"

namespace GDSA::Geometry
{
bool Circle::isInside(Point& point)
{
    if(this->_center.distance(point) <= this->_radius)
    {
        return true;
    }
    return false;
}

Polygon Circle::getPointsCircle(const uint32_t numSubdivisions) const
{
    Polygon      polygon;
    double       angle = 0.0;
    const double alpha = 2.0 * glm::pi<double>() / static_cast<double>(numSubdivisions);

    while(angle < (2.0 * glm::pi<double>() + BasicGeometry::EPSILON))
    {
        Point point = Point(std::cos(angle), -std::sin(angle));
        polygon.add(point);
        angle += alpha;
    }

    return polygon;
}

CircleCircleRelations Circle::relation(const Circle& other) const
{
    Point*       otherCenter            = new Point(other.getCenter().getX(), other.getCenter().getY());
    const double distanceBetweenCenters = this->getCenter().distance(*otherCenter);

    double r1, r2;
    if(this->getRadius() > other.getRadius())
    {
        r1 = this->getRadius();
        r2 = other.getRadius();
    }
    else
    {
        r2 = this->getRadius();
        r1 = other.getRadius();
    }

    if(BasicGeometry::equal(distanceBetweenCenters, 0.0))
    {
        return CircleCircleRelations::CONCENTRIC;
    }
    else if(r1 - r2 < distanceBetweenCenters && distanceBetweenCenters < r1 + r2)
    {
        return CircleCircleRelations::SECANT;
    }
    else if(distanceBetweenCenters > r1 + r2)
    {
        return CircleCircleRelations::EXTERNAL;
    }
    else if(distanceBetweenCenters < r1 + r2)
    {
        return CircleCircleRelations::INTERNAL;
    }
    else if(BasicGeometry::equal(distanceBetweenCenters, r1 + r2))
    {
        return CircleCircleRelations::EXTERNAL_TANG;
    }
    else if(BasicGeometry::equal(distanceBetweenCenters, (r1 - r2)))
    {
        return CircleCircleRelations::INTERIOR_TANG;
    }

    // Should never reach this point
    return CircleCircleRelations::NO_RELATION;
}

CircleLineRelations Circle::relation(Line& line) const
{
    auto*      centralPoint         = new Vect2d(this->getCenter().getX(), this->getCenter().getY());
    const auto radius               = this->getRadius();
    const auto lineToCenterDistance = line.distancePointLine(*centralPoint);

    if(lineToCenterDistance < radius)
    {
        return CircleLineRelations::INTERSECT;
    }
    else if(BasicGeometry::equal(radius, lineToCenterDistance))
    {
        return CircleLineRelations::TANGENT;
    }
    return CircleLineRelations::NO_INTERSECT;
}

CircleLineRelations Circle::intersect(Line& line, Point& p1, Point& p2) const
{
    Vect2d* d      = new Vect2d((line.getB() - line.getA()).getX(), (line.getB() - line.getA()).getY());
    Vect2d* var    = new Vect2d((line.getA() - this->getCenter()).getX(), (line.getA() - this->getCenter()).getY());
    Point*  circle = new Point(this->getPointsCircle().getVertexAt(0).getX() * this->getRadius() + this->getCenter().getX(),
                               this->getPointsCircle().getVertexAt(0).getY() * this->getRadius() + this->getCenter().getY());

    const double r     = std::pow((*circle - this->getCenter()).getModule(), 2);
    double       gamma = std::pow(d->dot(*var), 2) - std::pow(d->getModule(), 2) * (std::pow(var->getModule(), 2) - r);
    if(BasicGeometry::equal(gamma, 0))
    {
        gamma = 0;
    }

    const double tMax = (-d->dot(*var) + sqrt(gamma)) / pow(d->getModule(), 2);
    const double tMin = (-d->dot(*var) - sqrt(gamma)) / pow(d->getModule(), 2);
    if(BasicGeometry::equal(gamma, 0))
    {
        p1 = line.getPoint(tMax);
        return CircleLineRelations::TANGENT;
    }
    else if(gamma > 0)
    {
        p1 = line.getPoint(tMax);
        p2 = line.getPoint(tMin);
        return CircleLineRelations::INTERSECT;
    }

    return CircleLineRelations::NO_INTERSECT;
}

CircleLineRelations Circle::intersect(RayLine& rayLine, Point& p1, Point& p2) const
{
    Vect2d* d      = new Vect2d((rayLine.getB() - rayLine.getA()).getX(), (rayLine.getB() - rayLine.getA()).getY());
    Vect2d* var    = new Vect2d((rayLine.getA() - this->getCenter()).getX(), (rayLine.getA() - this->getCenter()).getY());
    Point*  circle = new Point(this->getPointsCircle().getVertexAt(0).getX() * this->getRadius() + this->getCenter().getX(),
                               this->getPointsCircle().getVertexAt(0).getY() * this->getRadius() + this->getCenter().getY());

    const double r     = std::pow((*circle - this->getCenter()).getModule(), 2);
    double       gamma = std::pow(d->dot(*var), 2) - std::pow(d->getModule(), 2) * (std::pow(var->getModule(), 2) - r);

    if(BasicGeometry::equal(gamma, 0))
    {
        gamma = 0;
    }

    const double tMax = (-d->dot(*var) + sqrt(gamma)) / pow(d->getModule(), 2);
    const double tMin = (-d->dot(*var) - sqrt(gamma)) / pow(d->getModule(), 2);
    if(BasicGeometry::equal(gamma, 0))
    {
        if(0 < tMax || BasicGeometry::equal(0.0, tMax))
        {
            p1 = rayLine.getPoint(tMax);
            return CircleLineRelations::TANGENT;
        }
        return CircleLineRelations::NO_INTERSECT;
    }
    else if(gamma > 0)
    {
        if(0 < tMax || BasicGeometry::equal(0.0, tMax))
        {
            p1 = rayLine.getPoint(tMax);
        }
        if(0 < tMin || BasicGeometry::equal(0.0, tMin))
        {
            p2 = rayLine.getPoint(tMin);
        }
        return CircleLineRelations::INTERSECT;
    }

    return CircleLineRelations::NO_INTERSECT;
}

CircleLineRelations Circle::intersect(SegmentLine& segment, Point& p1, Point& p2)
{
    Vect2d* d      = new Vect2d((segment.getB() - segment.getA()).getX(), (segment.getB() - segment.getA()).getY());
    Vect2d* var    = new Vect2d((segment.getA() - this->getCenter()).getX(), (segment.getA() - this->getCenter()).getY());
    Point*  circle = new Point(this->getPointsCircle().getVertexAt(0).getX() * this->getRadius() + this->getCenter().getX(),
                               this->getPointsCircle().getVertexAt(0).getY() * this->getRadius() + this->getCenter().getY());

    const double r     = std::pow((*circle - this->getCenter()).getModule(), 2);
    double       gamma = std::pow(d->dot(*var), 2) - std::pow(d->getModule(), 2) * (std::pow(var->getModule(), 2) - r);
    if(BasicGeometry::equal(gamma, 0))
    {
        gamma = 0;
    }

    const double tMax = (-d->dot(*var) + sqrt(gamma)) / pow(d->getModule(), 2);
    const double tMin = (-d->dot(*var) - sqrt(gamma)) / pow(d->getModule(), 2);
    if(BasicGeometry::equal(0.0, gamma))
    {
        if((0 < tMax && tMax < 1) || BasicGeometry::equal(0.0, tMax) || BasicGeometry::equal(1.0, tMax))
        {
            p1 = segment.getPoint(tMax);
            return CircleLineRelations::TANGENT;
        }
        return CircleLineRelations::NO_INTERSECT;
    }
    else if(gamma > 0)
    {
        if((0 < tMax && tMax < 1) || BasicGeometry::equal(0.0, tMax) || BasicGeometry::equal(1.0, tMax))
        {
            p1 = segment.getPoint(tMax);
        }
        if((0 < tMin && tMin < 1) || BasicGeometry::equal(0.0, tMin) || BasicGeometry::equal(1.0, tMin))
        {
            p2 = segment.getPoint(tMin);
        }
        return CircleLineRelations::INTERSECT;
    }
    return CircleLineRelations::NO_INTERSECT;
}

Circle& Circle::operator=(const Circle& circle)
{
    if(this != &circle)
    {
        this->_center = circle._center;
        this->_radius = circle._radius;
    }

    return *this;
}
}    // namespace GDSA::Geometry
