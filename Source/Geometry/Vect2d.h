#pragma once

#include "Point.h"

namespace GDSA::Geometry
{
class Vect2d : public Point
{
public:
    Vect2d() = default;
    Vect2d(double x, double y);
    Vect2d(const Vect2d& vector);
    Vect2d(const Point& point);

    ~Vect2d() override = default;

    Vect2d& operator=(const Vect2d& vector) = default;

    /**
     *  @return X coordinate.
     */
    double getX() override;

    /**
     *  @return Y coordinate.
     */
    double getY() override;

    /**
     *  @brief Returns a . b (scalar product).
     */
    double dot(const Vect2d& b) const;

    /**
     *  @brief Returns this + b (sum of vectors).
     */
    Vect2d operator+(const Vect2d& b);
    Vect2d operator+=(const Vect2d& b);

    /**
     *  @brief Returns a - b (substraction of vectors).
     */
    Vect2d operator-(const Vect2d& b);

    /**
     *  @brief Returns t . a (vector product by a scalar).
     */
    Vect2d ScalarMult(double t);
};
}
