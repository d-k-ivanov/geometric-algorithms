#pragma once

#include "Model3D.h"

#include "Geometry/Bezier.h"
#include "Geometry/Line.h"

namespace GDSA::Render
{
class DrawBezier : public Model3D
{
protected:
    Geometry::Bezier _bezier;

public:
    DrawBezier(const Geometry::Bezier& bezier, double lertStep);
    DrawBezier(const DrawBezier& drawBezier) = delete;
    ~DrawBezier() override { }
};
}    // namespace GDSA::Render
