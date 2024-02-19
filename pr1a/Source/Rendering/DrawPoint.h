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
    DrawPoint(const Point& p);
    DrawPoint(const DrawPoint& drawPoint) = delete;
    ~DrawPoint() override                 = default;
};
}    // namespace AlgGeom
