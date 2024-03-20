#pragma once

#include "Vect2d.h"

namespace GDSA::Geometry
{
class Triangle
{
protected:
    Vect2d _a, _b, _c;

public:
    friend class DrawTriangle;    // avoid

    Triangle(const Vect2d& a, const Vect2d& b, const Vect2d& c);
    Triangle(const Triangle& t) = default;

    /**
     *  @brief getter.
     *  @return vertex A.
     */
    Vect2d getA() { return _a; }

    /**
     *  @brief getter.
     *   @return vertex B.
     */
    Vect2d getB() { return _b; }

    /**
     *   @brief getter.
     *   @return vertex C.
     */
    Vect2d getC() { return _c; }
};
}    // namespace GDSA::Geometry
