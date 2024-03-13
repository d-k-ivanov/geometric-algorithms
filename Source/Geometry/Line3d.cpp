#include "Line3d.h"
#include "BasicGeometry.h"

#include <iostream>

Line3d::Line3d()
{
}

Line3d::Line3d(Vect3d& orig, Vect3d& dest)
    : Edge3d(orig, dest)
{
}

Line3d::Line3d(const Line3d& line)
    : Edge3d(line)
{
}

Line3d::~Line3d()
{
}

double Line3d::distance(Line3d& line)
{
    // XXXX
    return 0;
}

Line3d Line3d::normalLine(Vect3d& point)
{
    // XXXX
    return Line3d();
}

double Line3d::distance(Vect3d& p)
{

    /// XXXX
    return 0;
}

Line3d& Line3d::operator=(const Line3d& line)
{
    if(this != &line)
    {
        Edge3d::operator=(line);
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Line3d& line)
{
    std::cout << "Line -> Origin: " << line._orig << ", Destination: " << line._dest;
    return os;
}
