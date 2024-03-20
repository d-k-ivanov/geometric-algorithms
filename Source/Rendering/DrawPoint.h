#pragma once

#include "Model3D.h"

#include "Geometry/Point.h"
#include "Geometry/Vect3d.h"

namespace GDSA::Render
{
class DrawPoint : public Model3D
{
protected:
    Geometry::Point _point;

public:
    DrawPoint(const Geometry::Point& p);
    DrawPoint(const Geometry::Vect3d& p);
    DrawPoint(const DrawPoint& drawPoint) = delete;
    ~DrawPoint() override                 = default;
};
}    // namespace GDSA::Render
