#include "Point.h"
#include "StdAfx.h"

Point::Point()
{
    _x = DEFAULT_VALUE;
    _y = DEFAULT_VALUE;
}

Point::Point(double x, double y, bool polar)
{
    if(!polar)
    {
        _x = x;
        _y = y;
    }
    else
    {
        // _x = std::sqrt(std::pow(x, 2) + std::pow(y, 2)) * std::cos(std::atan2(x, y));
        // _y = std::sqrt(std::pow(x, 2) + std::pow(y, 2)) * std::sin(std::atan2(x, y));

        _x = y * std::cos(x);
        _y = y * std::sin(x);
    }
}

Point::Point(const Point& point)
{
    _x = point._x;
    _y = point._y;
}

Point::~Point()
{
}

Point::PointClassification Point::classify(Point& p0, Point& p1)
{
    Point        p2 = *this;
    Point        a  = p1 - p0;
    Point        b  = p2 - p0;
    const double sa = a._x * b._y - b._x * a._y;

    if(sa > 0.0)
    {
        return PointClassification::LEFT;
    }

    if(sa < 0.0)
    {
        return PointClassification::RIGHT;
    }

    if((a._x * b._x < 0.0) || (a._y * b._y < 0.0))
    {
        return PointClassification::BACKWARD;
    }

    if(a.getModule() < b.getModule())
    {
        return PointClassification::FORWARD;
    }

    if(p0.equal(p2))
    {
        return PointClassification::ORIGIN;
    }

    if(p1.equal(p2))
    {
        return PointClassification::DEST;
    }

    return PointClassification::BETWEEN;
}

bool Point::colinear(Point& a, Point& b)
{
    PointClassification result = classify(a, b);
    return (result != PointClassification::LEFT) && (result != PointClassification::RIGHT);
}

double Point::distance(Point& p)
{
    return std::sqrt(std::pow(p._x - _x, 2) + std::pow(p._y - _y, 2));
}

double Point::getAlpha()
{
    return std::atan2(_y, _x);
}

double Point::getModule()
{
    return std::sqrt(std::pow(_x, 2) + std::pow(_y, 2));
}

bool Point::leftAbove(Point& a, Point& b)
{
    PointClassification result = classify(a, b);
    return (result == PointClassification::LEFT) || (result != PointClassification::RIGHT);
}

Point& Point::operator=(const Point& point)
{
    _x = point._x;
    _y = point._y;

    return *this;
}

Point& Point::operator-(const Point& point)
{
    _x -= point._x;
    _y -= point._y;

    return *this;
}

Point& Point::operator+(const Point& point)
{
    _x += point._x;
    _y += point._y;

    return *this;
}

Point& Point::operator*(double scalar)
{
    _x *= scalar;
    _y *= scalar;

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Point& point)
{
    os << "Coordinate X: " << point._x << ", coordinate Y: " << point._y;

    return os;
}

bool Point::rightAbove(Point& a, Point& b)
{
    PointClassification result = classify(a, b);
    return (result == PointClassification::RIGHT) || (result != PointClassification::LEFT);
}

double Point::slope(Point& p)
{
    if(std::abs(p._x - _x) < std::numeric_limits<double>::epsilon())
    {
        INFINITY;
    }

    return (p._y - _y) / (p._x - _x);
}

double Point::triangleArea2(Point& a, Point& b)
{
    return _x * a._y - _y * a._x + a._x * b._y - a._y * b._x + b._x * _y - b._y * _x;
}

Point operator*(double scalar, Point& point)
{
    return scalar * point;
}
