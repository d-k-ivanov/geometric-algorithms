#pragma once

#include "Model3D.h"

#include "Geometry/Line.h"
#include "Geometry/Line3d.h"

namespace AlgGeom
{
class DrawLine : public Model3D
{
public:
    DrawLine(Line& line);
    DrawLine(Line3d& line);
    DrawLine(const DrawLine& drawLine) = delete;
    virtual ~DrawLine() { }
};
}    // namespace AlgGeom
