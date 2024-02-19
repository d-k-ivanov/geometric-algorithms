#pragma once

#include "Model3D.h"

#include "Geometry/Point.h"

namespace AlgGeom
{
class DrawPoint : public Model3D
{
protected:
    Point _point;

public:
    DrawPoint(Point& p);
    DrawPoint(const DrawPoint& drawPoint) = delete;
    virtual ~DrawPoint() {};
};
}
