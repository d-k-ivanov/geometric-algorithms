#pragma once

#include "Model3D.h"

#include "Geometry/Circle.h"

namespace AlgGeom
{
class DrawCircle : public Model3D
{
protected:
    Circle   _circle;
    uint32_t _numSubdivisions;

public:
    DrawCircle(Circle& circle, uint32_t numSubdivisions = 256);
    DrawCircle(const DrawCircle& circle) = delete;
    ~DrawCircle() override               = default;
};
}
