#include "Bezier.h"

#include <utility>

namespace GDSA::Geometry
{
Bezier::Bezier(std::vector<Vect2d> controlPoints, const int controlPointsNum)
    : _controlPoints(std::move(controlPoints))
    , _controlPointsNum(controlPointsNum)
{
}

Bezier::Bezier(const std::vector<Point>& controlPoints, const int controlPointsNum)
{
    _controlPointsNum = controlPointsNum;
    for(const auto& point : controlPoints)
    {
        _controlPoints.emplace_back(point);
    }
}

Bezier::~Bezier() = default;

void Bezier::add(const Point& point)
{
    _controlPoints.emplace_back(point);
    _controlPointsNum++;
}

void Bezier::add(const Vect2d& vector)
{
    _controlPoints.push_back(vector);
    _controlPointsNum++;
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

Vect2d Bezier::getOrigin() const
{
    return _controlPoints[0];
}

Vect2d Bezier::getEnd() const
{
    return _controlPoints[_controlPoints.size() - 1];
}

std::vector<Vect2d> Bezier::getControlPoints()
{
    return _controlPoints;
}

Vect2d Bezier::getControlPoint(size_t index) const
{
    return _controlPoints[index];
}

int Bezier::getControlPointsNum() const
{
    return _controlPointsNum;
}

int Bezier::getOrder() const
{
    return _controlPointsNum - 1;
}

Vect2d Bezier::getPoint(const double t)
{
    const auto curveOrder = this->getOrder();
    Vect2d     result     = this->getAt(0) * this->getBinomialCoefficient(curveOrder, 0) * std::pow(1 - t, _controlPointsNum - 1);
    for(int i = 1; i < _controlPointsNum; i++)
    {
        const auto binomialCoefficient  = this->getBinomialCoefficient(curveOrder, i);
        const auto bershteinCoefficient = std::pow(t, i) * std::pow(1 - t, curveOrder - i);
        result += this->getAt(i) * binomialCoefficient * bershteinCoefficient;
    }
    return result;
}

double Bezier::getBinomialCoefficient(const int n, const int k) const
{
    if(k < 0 || k > n)
    {
        return 0;
    }

    if(k == 0 || k == n)
    {
        return 1;
    }

    if(k == 1 || k == n)
    {
        return n;
    }

    return this->getBinomialCoefficient(n, k - 1) * (n + 1 - k) / k;
}
}    // namespace GDSA::Geometry
