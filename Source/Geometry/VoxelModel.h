#pragma once

#include "Triangle3d.h"
#include "Vect3d.h"

#include "iostream"
#include "vector"

namespace GDSA::Geometry
{
enum VoxelStatus
{
    OUTER    = 0,
    OCCUPIED = 1,
    INNER    = 2,
    NP       = 3
};

class VoxelModel
{
private:
    VoxelStatus         _status;
    std::vector<Vect3d> _points;
    Vect3d              _center, _min, _max;
    glm::vec3           _size;

public:
    VoxelModel();
    ~VoxelModel() = default;
    VoxelModel(const VoxelModel& voxel);
    VoxelModel(const Vect3d& minPoint, glm::vec3 size);
    void add(const Vect3d& point);
    bool find(const Vect3d& point);
    bool remove(const Vect3d& point);

    void setStatus(VoxelStatus status) { _status = status; }

    Vect3d      getMin() { return this->_min; }
    Vect3d      getMax() { return this->_max; }
    Vect3d      getCenter() { return this->_center; }
    VoxelStatus getStatus() const { return this->_status; }
    glm::vec3   getSize() const { return this->_size; }

    bool bruteForce(Triangle3d triangle);

    bool planeBoxOverlap(const Vect3d& normal, const Vect3d& vertex, glm::vec3 maxPoint) const;
    bool triangleOverlap(const Vect3d& center, glm::vec3 size, std::vector<Vect3d> vertices) const;

    bool xAxisTest(const double& a, const double& b, const double& fa, const double& fb, double& p0, double& px, const Vect3d& v0, const Vect3d& vx, const glm::vec3& boxRadius, double& min, double& max, double& rad) const;
    bool yAxisTest(const double& a, const double& b, const double& fa, const double& fb, double& p0, double& px, const Vect3d& v0, const Vect3d& vx, const glm::vec3& boxRadius, double& min, double& max, double& rad) const;
    bool zAxisTest(const double& a, const double& b, const double& fa, const double& fb, double& p0, double& px, const Vect3d& v0, const Vect3d& vx, const glm::vec3& boxRadius, double& min, double& max, double& rad) const;
};
}    // namespace GDSA::Geometry
