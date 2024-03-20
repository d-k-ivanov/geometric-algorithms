#pragma once

#include "Model3D.h"

#include "Geometry/RayLine.h"
#include "Geometry/Ray3d.h"

namespace GDSA
{
class DrawRay : public Model3D
{
public:
    DrawRay(RayLine& ray);
    DrawRay(Ray3d& ray);
    DrawRay(const DrawRay& drawRay) = delete;
    ~DrawRay() override {};
};
}    // namespace GDSA
