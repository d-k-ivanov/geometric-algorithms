#include "Polygon.h"

#include <fstream>
#include <sstream>

namespace GDSA::Geometry
{
Polygon::Polygon()
{
}

Polygon::Polygon(const Polygon& Polygon)
{
    _vertices = std::vector<Vertex>(Polygon._vertices);
}

Polygon::Polygon(std::vector<Vertex>& vertices)
{
    _vertices = std::vector<Vertex>(vertices);
}

SegmentLine Polygon::getEdge(int i)
{
    return SegmentLine(getVertexAt(i), getVertexAt((i + 1) % _vertices.size()));
}

size_t Polygon::getNumVertices()
{
    return _vertices.size();
}

Polygon::Polygon(const std::string& filename)
{
    std::ifstream file(filename);

    int numVertices;
    file.read(reinterpret_cast<char*>(&numVertices), sizeof(int));
    std::vector<Vertex> vertices;
    for(int i = 0; i < numVertices; i++)
    {
        Vertex vertex;
        file.read(reinterpret_cast<char*>(&vertex), sizeof(Vertex));
        vertices.push_back(vertex);
    }
    file.close();

    this->_vertices = vertices;
}

Polygon::~Polygon()
{
}

std::vector<Vertex>& Polygon::getVertices()
{
    return _vertices;
}

bool Polygon::add(const Vertex& vertex)
{
    size_t index = _vertices.size();

    // if (intersectsWithAnySegment(vertex)) return false;

    _vertices.push_back(vertex);
    _vertices[index].setPolygon(this);
    _vertices[index].setPosition(static_cast<int>(index));

    return true;
}

bool Polygon::add(const Point& point)
{
    const Vertex vertex(point);

    return this->add(vertex);
}

Vertex Polygon::getVertexAt(size_t position)
{
    if(position < _vertices.size())
    {
        return _vertices[position];
    }
    else
    {
        return Vertex();
    }
}

bool Polygon::intersects(Line& line, std::vector<Vect2d>& intersections)
{
    bool result = false;
    for(int i = 0; i < this->getNumVertices(); ++i)
    {
        Vect2d intersection;
        if(this->getEdge(i).intersects(line, intersection))
        {
            intersections.push_back(intersection);
            result = true;
        }
    }
    return result;
}

bool Polygon::intersects(RayLine& ray, std::vector<Vect2d>& intersections)
{
    bool result = false;
    for(int i = 0; i < this->getNumVertices(); ++i)
    {
        Vect2d intersection;
        if(this->getEdge(i).intersects(ray, intersection))
        {
            intersections.push_back(intersection);
            result = true;
        }
    }
    return result;
}

bool Polygon::intersects(SegmentLine& segment, std::vector<Vect2d>& intersections)
{
    bool result = false;
    for(int i = 0; i < this->getNumVertices(); ++i)
    {
        Vect2d intersection;
        if(this->getEdge(i).intersects(segment, intersection))
        {
            intersections.push_back(intersection);
            result = true;
        }
    }
    return result;
}

bool Polygon::convex()
{
    for(size_t i = 0; i < this->getNumVertices(); i++)
    {
        if(this->_vertices[i].concave())
        {
            return false;
        }
    }

    return true;
}

Vertex Polygon::next(size_t index)
{
    if(index < _vertices.size())
    {
        return _vertices[(index + 1) % _vertices.size()];
    }

    return {};
}

std::ostream& operator<<(std::ostream& os, const Polygon& polygon)
{
    for(int i = 0; i < polygon._vertices.size(); i++)
    {
        os << polygon._vertices[i] << "\n";
    }

    return os;
}

Vertex Polygon::previous(int index)
{
    if(index >= 0 && index < _vertices.size())
    {
        return _vertices[(index - 1 + _vertices.size()) % _vertices.size()];
    }

    return Vertex();
}

Polygon& Polygon::operator=(const Polygon& polygon)
{
    if(this != &polygon)
    {
        this->_vertices = polygon._vertices;
    }

    return *this;
}

bool Polygon::pointInConvexPolygonGeo(Point& point)
{
    for(size_t i = 0; i < this->getNumVertices(); i++)
    {
        Vertex vertex = this->getVertexAt(i);
        Vertex next   = this->next(i);
        if(point.triangleArea2(vertex, next) < 0)
        {
            return false;
        }
    }

    return true;
}

void Polygon::save(const std::string& filename)
{
    std::ofstream file(filename);

    int numVertices = static_cast<int>(this->getNumVertices());
    file.write(reinterpret_cast<char*>(&numVertices), sizeof(int));
    file.write(reinterpret_cast<char*>(_vertices.data()), static_cast<int>(numVertices * sizeof(Vertex)));
    file.close();
}

void Polygon::set(Vertex& vertex, int pos)
{
    if(pos >= 0 && pos < static_cast<int>(_vertices.size()))
    {
        _vertices[pos] = vertex;
        vertex.setPolygon(this);
        vertex.setPosition(pos);
    }
}
} // namespace GDSA::Geometry
