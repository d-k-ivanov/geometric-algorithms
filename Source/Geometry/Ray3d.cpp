#include "Ray3d.h"
#include "Edge3d.h"

#include <iostream>

namespace GDSA::Geometry
{
Ray3d::Ray3d(Vect3d& orig, Vect3d& dest)
    : Edge3d(orig, dest)
{
}

Ray3d::Ray3d(const Ray3d& ray)
    : Edge3d(ray)
{
}

Ray3d::~Ray3d()
{
}

Ray3d& Ray3d::operator=(const Ray3d& ray)
{
    if(this != &ray)
    {
        Edge3d::operator=(ray);
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Ray3d& ray)
{
    os << "Ray --> Origin: " << ray._orig << "; destination: " << ray._dest;
    return os;
}
}    // namespace GDSA::Geometry
