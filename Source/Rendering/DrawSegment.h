#pragma once

#include "Model3D.h"

#include "Geometry/Segment3d.h"
#include "Geometry/SegmentLine.h"

namespace GDSA::Render
{
class DrawSegment : public Model3D
{
public:
    DrawSegment(Geometry::SegmentLine& segment);
    DrawSegment(Geometry::Segment3d& segment);
    DrawSegment(const DrawSegment& drawSegment) = delete;
    ~DrawSegment() override { }
};
}    // namespace GDSA::Render
