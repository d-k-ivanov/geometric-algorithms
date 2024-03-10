#pragma once

#include "Model3D.h"

#include "Geometry/RayLine.h"

namespace AlgGeom
{
class DrawRay : public Model3D
{
protected:
    RayLine _ray;

public:
    DrawRay(RayLine& t);
    DrawRay(const DrawRay& drawRay) = delete;
    ~DrawRay() override {};
};
}    // namespace AlgGeom
