#include "Bezier.h"
#include "StdAfx.h"

Bezier::Bezier()
{
    _controlPoints = std::vector<Vect2d>();
}

Bezier::Bezier(const Bezier& bezier)
{
    _controlPoints = std::vector(bezier._controlPoints);
}

Bezier::Bezier(std::vector<Vect2d> vertices, int vertexNum)
{
    _vertexNum     = vertexNum;
    _controlPoints = std::vector(std::move(vertices));
}

Bezier::~Bezier() = default;

void Bezier::add(const Point& point)
{
    const Vect2d vector(point);
    _controlPoints.push_back(vector);
}

void Bezier::add(const Vect2d& vector)
{
    _controlPoints.push_back(vector);
}

Vect2d Bezier::getAt(const size_t position)
{
    if(position < _controlPoints.size())
    {
        return _controlPoints[position];
    }

    return {};
}

void Bezier::setAt(const size_t position, const Vect2d& vector)
{
    if(position < _controlPoints.size())
    {
        _controlPoints[position] = vector;
    }
}

std::vector<Vect2d> Bezier::getControlPoints()
{
    return _controlPoints;
}
