#include "StdAfx.h"

#include "Triangle.h"

Triangle::Triangle(Vect2d& aa, Vect2d& bb, Vect2d& cc)
    : _a(aa)
    , _b(bb)
    , _c(cc)
{
}

Triangle::Triangle(const Triangle& tt)
    : _a(tt._a)
    , _b(tt._b)
    , _c(tt._c)
{
}
