#pragma once

#include "Model3D.h"

#include "Geometry/Line.h"
#include "Geometry/Line3d.h"

namespace GDSA::Render
{
class DrawLine : public Model3D
{
public:
    DrawLine(Geometry::Line& line);
    DrawLine(Geometry::Line3d& line);
    DrawLine(const DrawLine& drawLine) = delete;
    ~DrawLine() override               = default;
};
}    // namespace GDSA::Render
