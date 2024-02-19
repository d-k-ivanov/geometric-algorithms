#pragma once

#include "Model3D.h"

#include "Geometry/Triangle.h"

namespace AlgGeom
{
class DrawTriangle : public Model3D
{
protected:
    Triangle _triangle;

public:
    DrawTriangle(Triangle& t);
    DrawTriangle(const DrawTriangle& drawTriangle) = delete;
    virtual ~DrawTriangle() {};
};
}    // namespace AlgGeom
