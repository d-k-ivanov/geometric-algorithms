#include "Edge3d.h"

#include <iostream>
#include <vector>

namespace GDSA::Geometry
{
Edge3d::Edge3d()
    : _orig(0.0f)
    , _dest(0.0f)
    , _direction(0.0f)
{
}

Edge3d::Edge3d(Vect3d& orig, Vect3d& dest)
    : _orig(orig)
    , _dest(dest)
    , _direction(_dest - _orig)
{
}

Edge3d::Edge3d(const Edge3d& edge)
{
    _orig      = edge._orig;
    _dest      = edge._dest;
    _direction = edge._direction;
}

Edge3d::~Edge3d()
{
}

Vect3d& Edge3d::getDestination()
{
    return _dest;
}

Vect3d& Edge3d::getOrigin()
{
    return _orig;
}

Vect3d Edge3d::getDirection() const
{
    return _direction;
}

Vect3d Edge3d::getPoint(double t)
{
    if(!isTvalid(t))
        return {};

    return _orig + _direction * t;
}

std::vector<double> Edge3d::getVertices() const
{
    std::vector<double> vertices;
    std::vector<double> origVertices = _orig.getVert(), destVertices = _dest.getVert();
    vertices.insert(vertices.end(), origVertices.begin(), origVertices.end());
    vertices.insert(vertices.end(), destVertices.begin(), destVertices.end());

    return vertices;
}

void Edge3d::setDest(const Vect3d& dest)
{
    _dest      = dest;
    _direction = _dest - _orig;
}

void Edge3d::setDirection(const Vect3d& direction)
{
    _direction = direction;
    _dest      = _orig + _direction;
}

void Edge3d::setOrigin(const Vect3d& orig)
{
    _orig      = orig;
    _direction = _dest - _orig;
}

Edge3d& Edge3d::operator=(const Edge3d& edge)
{
    _orig      = edge._orig;
    _dest      = edge._dest;
    _direction = edge._direction;

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Edge3d& edge)
{
    os << "Edge --> Origin: " << edge._orig << "; destination: " << edge._dest;
    return os;
}
}    // namespace GDSA::Geometry
