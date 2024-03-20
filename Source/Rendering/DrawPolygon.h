#pragma once

#include "Model3D.h"

#include "Geometry/Polygon.h"

namespace GDSA::Render
{
class DrawPolygon : public Model3D
{
protected:
    Geometry::Polygon _polygon;

public:
    DrawPolygon(Geometry::Polygon& polygon);
    DrawPolygon(const DrawPolygon& ddt) = delete;
    ~DrawPolygon() override             = default;
};
}    // namespace GDSA::Render
