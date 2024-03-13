#pragma once

#include "Model3D.h"

#include "Geometry/Point.h"
#include "Geometry/Vect3d.h"

namespace AlgGeom
{
class DrawPoint : public Model3D
{
protected:
    Point _point;

public:
    DrawPoint(const Point& p);
    DrawPoint(const Vect3d& p);
    DrawPoint(const DrawPoint& drawPoint) = delete;
    ~DrawPoint() override                 = default;
};
}    // namespace AlgGeom
