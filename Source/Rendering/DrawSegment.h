#pragma once

#include "Model3D.h"

#include "Geometry/Segment3d.h"
#include "Geometry/SegmentLine.h"

namespace AlgGeom
{
class DrawSegment : public Model3D
{
public:
    DrawSegment(SegmentLine& segment);
    DrawSegment(Segment3d& segment);
    DrawSegment(const DrawSegment& drawSegment) = delete;
    ~DrawSegment() override { }
};
}    // namespace AlgGeom
