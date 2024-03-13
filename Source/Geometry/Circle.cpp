#include "Circle.h"

Circle::~Circle()
{
}

bool Circle::isInside(const Point& p)
{
    // XXXX
    return true;
}

Polygon Circle::getPointsCircle(uint32_t numSubdivisions)
{
    Polygon     polygon;
    double       angle = .0f;
    const double alpha = 2.0f * glm::pi<double>() / static_cast<double>(numSubdivisions);

    while(angle < (2.0f * glm::pi<double>() + glm::epsilon<double>()))
    {
        Point point = Point(std::cos(angle), -std::sin(angle));
        polygon.add(point);
        angle += alpha;
    }

    return polygon;
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
