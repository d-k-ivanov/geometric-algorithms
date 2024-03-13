#pragma once

#include "Model3D.h"

#include "Geometry/Segment3d.h"
#include "Geometry/SegmentLine.h"

namespace AlgGeom
{
class DrawSegment : public Model3D
{
public:
    DrawSegment(SegmentLine& t);
    DrawSegment(Segment3d& t);
    DrawSegment(const DrawSegment& drawSegment) = delete;
    ~DrawSegment() override { }
};
}    // namespace AlgGeom
