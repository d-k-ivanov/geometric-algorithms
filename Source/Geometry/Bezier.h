#pragma once

#include <Geometry/Vect2d.h>

#include <vector>

class Bezier
{
protected:
    std::vector<Vect2d> _controlPoints;
    int                 _controlPointsNum = 0;

public:
    Bezier() = default;
    Bezier(const Bezier& bezier) = default;
    Bezier(std::vector<Vect2d> controlPoints, int controlPointsNum);
    Bezier(const std::vector<Point>& controlPoints, int controlPointsNum);
    ~Bezier();

    void add(const Point& point);
    void add(const Vect2d& vector);

    Vect2d getAt(size_t position);
    void   setAt(size_t position, const Vect2d& vector);

    Vect2d getOrigin() const;
    Vect2d getEnd() const;

    std::vector<Vect2d> getControlPoints();
    Vect2d              getControlPoint(size_t index) const;
    int                 getControlPointsNum() const;
    int                 getOrder() const;
    Vect2d              getPoint(double t);

private:
    double getBinomialCoefficient(int n, int k) const;
};
