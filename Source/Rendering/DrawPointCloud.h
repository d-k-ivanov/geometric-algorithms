#pragma once

#include "Model3D.h"

#include "Geometry/PointCloud.h"
#include "Geometry/PointCloud3d.h"

namespace GDSA
{
class DrawPointCloud : public Model3D
{
public:
    DrawPointCloud(PointCloud& pointCloud);
    DrawPointCloud(PointCloud3d& pointCloud);
    DrawPointCloud(const DrawPointCloud& drawPointCloud) = delete;
    ~DrawPointCloud() override;
};
}    // namespace GDSA
