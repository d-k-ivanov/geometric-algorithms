#pragma once
#include "PointCloud.h"
#include "Segment3d.h"
#include "Triangle.h"
#include "TriangleModel.h"

namespace GDSA::Geometry
{
class ConvexHull
{
private:
    std::vector<Vect2d>     _pointSequence2D;
    std::vector<Vect3d>     _pointSequence3D;
    std::vector<Triangle>   _triangles2D;
    std::vector<Triangle3d> _triangles3D;
    std::vector<Segment3d>  _segments3D;

public:
    ConvexHull() = default;
    ConvexHull(PointCloud* pointCloud);
    ConvexHull(PointCloud3d* pointCloud);
    ConvexHull(TriangleModel* model);

    ~ConvexHull() = default;

    const std::vector<Vect2d>&     getPoints2D() const { return _pointSequence2D; }
    const std::vector<Vect3d>&     getPoints3D() const { return _pointSequence3D; }
    const std::vector<Triangle>&   getTriangles2D() const { return _triangles2D; }
    const std::vector<Triangle3d>& getTriangles3D() const { return _triangles3D; }
    const std::vector<Segment3d>&  getSegments() const { return _segments3D; }
};
}    // namespace GDSA::Geometry
