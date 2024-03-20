#include "Vertex.h"

#include "Polygon.h"

#include <string>

namespace GDSA::Geometry
{
Vertex::Vertex()
    : Point()
{
    _position = INVALID_POSITION;
    _polygon  = nullptr;
}

Vertex::Vertex(const Point& point)
    : Point(point)
{
    _position = INVALID_POSITION;
    _polygon  = nullptr;
}

Vertex::Vertex(const Point& point, Polygon* polygon, int pos)
    : Point(point)
{
    _position = pos;
    _polygon  = polygon;
}

Vertex::~Vertex()
{
}

bool Vertex::convex()
{
    Vertex prevV = this->previous();
    Vertex nextV = this->next();

    if(prevV.isValid() && nextV.isValid())
    {
        return nextV.left(prevV, *this);
    }

    return false;
}

bool Vertex::concave()
{
    return !this->convex();
}

Vertex Vertex::next()
{
    if(_polygon && _position != INVALID_POSITION)
    {
        return _polygon->next(_position);
    }
    return {};
}

SegmentLine Vertex::nextEdge()
{
    Vertex nextV = this->next();

    if(nextV.isValid())
    {
        return {*this, nextV};
    }

    return {};
}

Vertex& Vertex::operator=(const Vertex& vertex)
{
    if(this != &vertex)
    {
        Point::operator=(vertex);
        this->_polygon  = vertex._polygon;
        this->_position = vertex._position;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Vertex& vertex)
{
    os << "Position: " << std::to_string(vertex._position);

    return os;
}

Vertex Vertex::previous()
{
    if(_polygon && _position != INVALID_POSITION)
    {
        return _polygon->previous(_position);
    }
    return {};
}

SegmentLine Vertex::previousEdge()
{
    Vertex prevV = this->previous();

    if(prevV.isValid())
    {
        return {prevV, *this};
    }

    return {};
}
}    // namespace GDSA::Geometry
