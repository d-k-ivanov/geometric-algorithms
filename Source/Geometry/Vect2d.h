#pragma once

#include "Point.h"

namespace GDSA::Geometry
{
class Vect2d : public Point
{
public:
    Vect2d();
    Vect2d(double x, double y);

    /**
     *  @brief Copy constructor.
     */
    Vect2d(const Vect2d& vector);

    /**
     *  @brief Copy constructor.
     */
    Vect2d(const Point& point);

    /**
     *  @brief Destructor.
     */
    ~Vect2d() override;

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
    double dot(Vect2d& b);

    /**
     *  @brief Assignment operator.
     */
    Vect2d& operator=(const Vect2d& vector);

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
    Vect2d scalarMult(double t);
};
}