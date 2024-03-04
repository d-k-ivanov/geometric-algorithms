#pragma once

#include "Model3D.h"

#include "Geometry/Bezier.h"

namespace AlgGeom
{
class DrawBezier : public Model3D
{
protected:
    Bezier _bezier;

public:
    DrawBezier(Bezier& bezier);
    DrawBezier(const DrawBezier& drawBezier) = delete;
    ~DrawBezier() override { }
};
}    // namespace AlgGeom
