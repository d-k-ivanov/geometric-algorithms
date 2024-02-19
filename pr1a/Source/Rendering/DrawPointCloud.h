#pragma once

#include "Model3D.h"

#include "Geometry/PointCloud.h"

namespace AlgGeom
{
class DrawPointCloud : public Model3D
{
protected:
    PointCloud _pointCloud;

public:
    DrawPointCloud(PointCloud& t);
    DrawPointCloud(const DrawPointCloud& drawPointCloud) = delete;
    virtual ~DrawPointCloud() {};
};
}
