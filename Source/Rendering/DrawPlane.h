#pragma once

#include "Model3D.h"

#include "Geometry/Plane.h"

namespace GDSA::Render
{
class DrawPlane : public Model3D
{
protected:
    Geometry::Plane _plane;

public:
    DrawPlane(Geometry::Plane& plane);
    DrawPlane(const DrawPlane& plane) = delete;
    ~DrawPlane() override             = default;
};
}    // namespace GDSA::Render
