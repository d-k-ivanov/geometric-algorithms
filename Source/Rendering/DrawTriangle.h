#pragma once

#include "Model3D.h"

#include "Geometry/Triangle.h"
#include "Geometry/Triangle3d.h"

namespace AlgGeom
{
class DrawTriangle : public Model3D
{
public:
    DrawTriangle(Triangle& triangle);
    DrawTriangle(Triangle3d& triangle);
    DrawTriangle(const DrawTriangle& drawTriangle) = delete;
    ~DrawTriangle() override                       = default;
};
}    // namespace AlgGeom
