#include "Triangle.h"

namespace GDSA::Geometry
{
Triangle::Triangle(const Vect2d& a, const Vect2d& b, const Vect2d& c)
    : _a(a)
    , _b(b)
    , _c(c)
{
}
}    // namespace GDSA::Geometry