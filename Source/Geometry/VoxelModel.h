#pragma once

#include "Triangle3d.h"
#include "Vect3d.h"

#include "iostream"
#include "vector"

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

    bool planeBoxOverlap(const Vect3d& normal, const Vect3d& vert, glm::vec3 maxPoint) const;
    bool triBoxOverlap(const Vect3d& center, glm::vec3 size, std::vector<Vect3d> vertices);

    bool axisTestX(double& a, double& b, float& fa, float& fb, float& p0, float& px, Vect3d& v0, Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad);
    bool axisTestY(double& a, double& b, float& fa, float& fb, float& p0, float& px, Vect3d& v0, Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad);
    bool axisTestZ(double& a, double& b, float& fa, float& fb, float& p0, float& px, Vect3d& v0, Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad);
};
