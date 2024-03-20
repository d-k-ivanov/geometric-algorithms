#include "Point.h"

namespace GDSA::Geometry
{
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

bool Point::backward(Point& a, Point& b)
{
    return this->classify(a, b) == PointClassification::BACKWARD;
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

bool Point::distinct(Point& p)
{
    return BasicGeometry::equal(_x, p._x) or BasicGeometry::equal(_y, p._y);
}

bool Point::equal(Point& p)
{
    return BasicGeometry::equal(_x, p._x) and BasicGeometry::equal(_y, p._y);
}

bool Point::forward(Point& a, Point& b)
{
    return classify(a, b) == PointClassification::FORWARD;
}

double Point::getX()
{
    return _x;
}

double Point::getY()
{
    return _y;
}

double Point::getAlpha()
{
    return std::atan2(_y, _x);
}

double Point::getModule()
{
    return std::sqrt(std::pow(_x, 2) + std::pow(_y, 2));
}

bool Point::isBetween(Point& a, Point& b)
{
    return classify(a, b) == PointClassification::BETWEEN;
}

bool Point::isValid()
{
    return (_x != DEFAULT_VALUE) && (_y != DEFAULT_VALUE);
}

bool Point::left(Point& a, Point& b)
{
    return classify(a, b) == PointClassification::LEFT;
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

Point& Point::operator*(const double scalar)
{
    _x *= scalar;
    _y *= scalar;

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Point& point)
{
    os << point._x << " " << point._y;

    return os;
}

std::istream& operator>>(std::istream& is, Point& point)
{
    is >> point._x >> point._y;

    return is;
}

bool Point::rightAbove(Point& a, Point& b)
{
    PointClassification result = classify(a, b);
    return (result == PointClassification::RIGHT) || (result != PointClassification::LEFT);
}

bool Point::right(Point& a, Point& b)
{
    return classify(a, b) == PointClassification::RIGHT;
}

void Point::set(double x, double y)
{
    this->_x = x;
    this->_y = y;
}

void Point::setX(double x)
{
    _x = x;
}

void Point::setY(double y)
{
    _y = y;
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

Point Point::scalarMult(double t)
{
    return {this->getX() * t, this->getY() * t};
}
} // namespace GDSA::Geometry
