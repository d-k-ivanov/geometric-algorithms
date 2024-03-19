#pragma once

#include "PointCloud.h"
#include "SegmentLine.h"

#include <vector>

class Triangulation
{
private:
    std::vector<std::pair<SegmentLine*, bool>> _edges;

public:
    Triangulation()  = default;
    ~Triangulation() = default;

    Triangulation(PointCloud* pointCloud);

    std::vector<std::pair<SegmentLine*, /*is segment*/ bool>> getEdges() const { return _edges; }
};
