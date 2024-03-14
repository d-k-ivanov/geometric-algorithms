#include "DrawCircle.h"

AlgGeom::DrawCircle::DrawCircle(Circle& circle, const uint32_t numSubdivisions)
    : Model3D()
    , _circle(circle)
    , _numSubdivisions(numSubdivisions)
{
    //
}
