#pragma once

#include "Point.h"

#include <string>
#include <vector>

namespace GDSA::Geometry
{
class PointCloud
{
protected:
    std::vector<Point> _points;

public:
    PointCloud() = default;
    PointCloud(int size, double maxX, double maxY);
    PointCloud(const std::vector<Point>& points);
    PointCloud(const std::string& filename);

    ~PointCloud() = default;

    PointCloud& operator=(const PointCloud& pointCloud);

    /**
     *  @brief Adds a point to the point cloud.
     */
    void addPoint(const Point& p);

    /**
     *  @brief Returns the points that minimizes the distance to any point from the cloud.
     */
    Point centralPoint();

    /**
     *  @brief Deletes a point identified by its index.
     */
    void deletePoint(int index);

    /**
     *  @brief Returns the point in an specific index.
     */
    Point getPoint(size_t position);

    /**
     *  @brief Returns all the available points as a vector.
     */
    std::vector<Point> getPoints() { return _points; }

    /**
     *  @brief Saves the cloud of points in file with the same format used by the constructor.
     */
    void save(const std::string& filename) const;

    /**
     *  @brief Returns the size of the point cloud.
     */
    size_t size() const { return _points.size(); }
};
}    // namespace GDSA::Geometry