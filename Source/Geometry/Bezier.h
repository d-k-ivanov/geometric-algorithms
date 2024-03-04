#pragma once

#include <Geometry/Point.h>
#include <Geometry/Vect2d.h>

class Bezier
{
protected:
    size_t              _vertexNum = 0;
    std::vector<Vect2d> _controlPoints;

public:
    Bezier();
    Bezier(const Bezier& bezier);
    Bezier(std::vector<Vect2d> vertices, int vertexNum);
    ~Bezier();

    void add(const Point& point);
    void add(const Vect2d& vector);

    Vect2d getAt(size_t position);
    void   setAt(size_t position, const Vect2d& vector);

    std::vector<Vect2d> getControlPoints();
};
