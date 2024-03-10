#pragma once

#include "Model3D.h"

#include "Geometry/Polygon.h"

namespace AlgGeom
{
class DrawPolygon : public Model3D
{
protected:
    Polygon _polygon;

public:
    DrawPolygon(Polygon& polygon);
    DrawPolygon(const DrawPolygon& ddt) = delete;
    ~DrawPolygon() override {};
};
}    // namespace AlgGeom
