#pragma once

#include "Vect2d.h"

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
    Triangle(Vect2d& aa, Vect2d& bb, Vect2d& cc);

    /**
     *   @brief Copy constructor.
     *   @param t.
     */
    Triangle(const Triangle& t);

    /**
     *	@brief getter.
     *	@return vertex A.
     */
    Vect2d getA() { return _a; }

    /**
     *	@brief getter.
     *   @return vertex B.
     */
    Vect2d getB() { return _b; }

    /**
     *   @brief getter.
     *   @return vertex C.
     */
    Vect2d getC() { return _c; }
};
