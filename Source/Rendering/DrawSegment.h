#pragma once

#include "Model3D.h"

#include "Geometry/SegmentLine.h"

namespace AlgGeom
{
class DrawSegment : public Model3D
{
protected:
    SegmentLine _segment;

public:
    DrawSegment(SegmentLine& t);
    DrawSegment(const DrawSegment& drawSegment) = delete;
    virtual ~DrawSegment() {};
};
}    // namespace AlgGeom
