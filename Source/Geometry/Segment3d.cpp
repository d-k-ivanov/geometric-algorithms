#include "Segment3d.h"

#include <iostream>

namespace GDSA::Geometry
{
Segment3d::Segment3d(Vect3d& orig, Vect3d& dest)
    : Edge3d(orig, dest)
{
}

Segment3d::Segment3d(const Segment3d& segment)
    : Edge3d(segment)
{
}

Segment3d::~Segment3d()
{
}

Segment3d& Segment3d::operator=(const Segment3d& segment)
{
    if(this != &segment)
    {
        Edge3d::operator=(segment);
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Segment3d& segment)
{
    std::cout << "Segment -> Origin: " << segment._orig << ", "
              << ", Destination: " << segment._dest;
    return os;
}
}    // namespace GDSA::Geometry
