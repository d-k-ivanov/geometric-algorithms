#pragma once

#include "Triangle3d.h"
#include "Vect3d.h"

#include "iostream"
#include "vector"

enum VoxelStatus
{
    OCCUPIED,
    OUTER,
    INNER,
    NP
};

class Voxel
{
private:
    VoxelStatus         _status;
    std::vector<Vect3d> _points;
    Vect3d              _center, _min, _max;
    glm::vec3           _size;

public:
    Voxel();
    ~Voxel() = default;
    Voxel(const Voxel& voxel);
    Voxel(Vect3d minPoint, glm::vec3 size);
    void add(Vect3d point);
    bool find(Vect3d point);
    bool remove(Vect3d point);

    void setFormato(VoxelStatus formato) { _status = formato; }

    Vect3d      getMin();
    Vect3d      getMax();
    VoxelStatus getStatus() const;

    bool bruteForce(Triangle3d triangle);
    bool lineSweep(Triangle3d triangle);
    bool AABBTri(Triangle3d triangulo);

    bool planeBoxOverlap(const Vect3d& normal, const Vect3d& vert, glm::vec3 maxPoint) const;
    bool triBoxOverlap(const Vect3d& center, glm::vec3 size, std::vector<Vect3d> vertices);

    bool axisTestX(double& a, double& b, float& fa, float& fb, float& p0, float& px, Vect3d& v0, Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad);
    bool axisTestY(double& a, double& b, float& fa, float& fb, float& p0, float& px, Vect3d& v0, Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad);
    bool axisTestZ(double& a, double& b, float& fa, float& fb, float& p0, float& px, Vect3d& v0, Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad);
};
