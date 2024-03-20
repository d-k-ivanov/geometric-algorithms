#pragma once

#include "Model3D.h"

#include "Geometry/PointCloud.h"
#include "Geometry/PointCloud3d.h"

namespace GDSA::Render
{
class DrawPointCloud : public Model3D
{
public:
    DrawPointCloud(Geometry::PointCloud& pointCloud);
    DrawPointCloud(Geometry::PointCloud3d& pointCloud);
    DrawPointCloud(const DrawPointCloud& drawPointCloud) = delete;
    ~DrawPointCloud() override;
};
}    // namespace GDSA::Render
