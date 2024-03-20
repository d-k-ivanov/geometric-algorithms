#pragma once

#include "AABB.h"
#include "Vect3d.h"

#include <string>

namespace GDSA::Geometry
{
/**
 *  @brief This class represents a set of points distributed in the space.
 */
class PointCloud3d
{
protected:
    std::vector<Vect3d> _points;
    Vect3d              _maxPoint, _minPoint;              // AABB.
    Vect3d              _maxPointIndex, _minPointIndex;    // Indices of those vertices which have the boundary coordinates of the mesh.

protected:
    /**
     *  @brief Updates the new maximum and minimum points taking into account a new point.
     */
    void updateMaxMin(size_t index);

public:
    PointCloud3d();
    PointCloud3d(const std::string& filename);
    PointCloud3d(const std::vector<Vect3d>& points);
    PointCloud3d(int size, double maxX, double maxY, double maxZ);
    PointCloud3d(int size, double radius);
    PointCloud3d(const PointCloud3d& pointCloud);

    virtual ~PointCloud3d() = default;

    PointCloud3d& operator=(const PointCloud3d& pointCloud);

    /**
     *  @brief Adds a new point to the cloud.
     */
    void addPoint(const Vect3d& p);

    /**
     *  @brief Removes all the points.
     */
    void clear() { _points.clear(); }

    /**
     *  @brief Returns the bounding box that delimites the point cloud space.
     */
    AABB getAABB() const;

    /**
     *    @brief get the index of the most distanced points in the cloud.
     */
    void getMostDistanced(int& a, int& b) const;

    /**
     *  @brief Returns a certain point.
     */
    Vect3d getPoint(size_t pos);

    /**
     *  @brief Returns all the cloud points.
     */
    std::vector<Vect3d> getPoints();

    /**
     *  @brief Saves the cloud points in a file.
     */
    void save(const std::string& filename) const;

    /**
     *  @brief Returns the number of points that this cloud contains.
     */
    size_t size() const { return _points.size(); }
};
}    // namespace GDSA::Geometry
