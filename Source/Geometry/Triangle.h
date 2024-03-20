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

    /**
     *   @brief Constructor .
     *   @param aa vertex.
     *   @param bb vertex.
     *   @param cc vertex.
     */
    Triangle(const Vect2d& aa, const Vect2d& bb, const Vect2d& cc);

    /**
     *   @brief Copy constructor.
     *   @param tt
     */
    Triangle(const Triangle& tt);

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
