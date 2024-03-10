#pragma once

#include "Model3D.h"

#include "Geometry/Bezier.h"
#include "Geometry/Line.h"

namespace AlgGeom
{
class DrawBezier : public Model3D
{
protected:
    Bezier _bezier;

public:
    DrawBezier(const Bezier& bezier, double lertStep);
    DrawBezier(const DrawBezier& drawBezier) = delete;
    ~DrawBezier() override { }
};
}    // namespace AlgGeom
