#pragma once

#include "Model3D.h"

#include "Geometry/Line.h"

namespace AlgGeom
{
class DrawLine : public Model3D
{
protected:
    Line _line;

public:
    DrawLine(Line& t);
    DrawLine(const DrawLine& drawLine) = delete;
    virtual ~DrawLine() {}
};
}    // namespace AlgGeom
