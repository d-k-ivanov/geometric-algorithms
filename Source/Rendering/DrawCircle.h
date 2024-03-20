#pragma once

#include "Model3D.h"

#include "Geometry/Circle.h"

namespace GDSA::Render
{
class DrawCircle : public Model3D
{
protected:
    Geometry::Circle   _circle;
    uint32_t _numSubdivisions;

public:
    DrawCircle(const Geometry::Circle& circle, uint32_t numSubdivisions = 256, bool drawTriangularSegments = true);
    DrawCircle(const DrawCircle& circle) = delete;
    ~DrawCircle() override               = default;
};
}
