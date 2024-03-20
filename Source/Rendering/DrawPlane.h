#pragma once

#include "Model3D.h"

#include "Geometry/Plane.h"

namespace GDSA
{
class DrawPlane : public Model3D
{
protected:
    Plane _plane;

public:
    DrawPlane(Plane& plane);
    DrawPlane(const DrawPlane& plane) = delete;
    ~DrawPlane() override;
};
}    // namespace GDSA
