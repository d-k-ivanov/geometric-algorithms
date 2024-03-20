#pragma once

#include "Model3D.h"

#include "Geometry/Ray3d.h"
#include "Geometry/RayLine.h"

namespace GDSA::Render
{
class DrawRay : public Model3D
{
public:
    DrawRay(Geometry::RayLine& ray);
    DrawRay(Geometry::Ray3d& ray);
    DrawRay(const DrawRay& drawRay) = delete;
    ~DrawRay() override {};
};
}    // namespace GDSA::Render
