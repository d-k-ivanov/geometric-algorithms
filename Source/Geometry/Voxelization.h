#pragma once

#include "Intersections3D.h"
#include "Ray3d.h"
#include "Triangle3d.h"
#include "TriangleModel.h"

#include "Rendering/DrawVoxelization.h"

class Voxelization
{
protected:
    class Voxel
    {
    public:
        enum VoxelStatus
        {
            OUTER    = 0,
            OCCUPIED = 1,
            INNER    = 2
        };

    public:
        AABB                         _aabb;
        VoxelStatus                  _status;
        std::unordered_set<unsigned> _triangles;

        Voxel()
            : _status(OUTER)
        {
        }
    };

    struct TriangleComparison
    {
        inline bool operator()(const Triangle3d& t1, const Triangle3d& t2) const
        {
            return t1.getAABB().getMin()._y < t2.getAABB().getMin()._y;
        }
    };

protected:
    AABB           _aabb;
    glm::vec3      _max, _min;
    TriangleModel* _model;
    glm::uvec3     _splits;
    Voxel***       _voxel;
    glm::vec3      _voxelSize;

public:
    struct VoxelHash
    {
        size_t operator()(const std::pair<glm::ivec3, Voxelization*> voxelPair) const
        {
            return static_cast<size_t>(
                voxelPair.first.x * voxelPair.second->_splits.y * voxelPair.second->_splits.z + voxelPair.first.y * voxelPair.second->_splits.z + voxelPair.first.z);
        }
    };

protected:
    void  flood(int x, int y, int z, std::unordered_set<std::pair<glm::ivec3, Voxelization*>, VoxelHash>& exploredVoxels);
    glm::ivec3 getIndices(const glm::vec3& point);
    void       insertPoint(const glm::vec3& point, unsigned triangleIdx);

public:
    Voxelization(TriangleModel* model, const glm::vec3& voxelSize);
    virtual ~Voxelization();

    void                       bruteForce() const;
    void                       flood();
    AlgGeom::DrawVoxelization* getRenderingObject(bool useColors = true) const;
    void                       printData() const;
    void                       voxelNeighbourhood();
    void                       sampleTriangle(unsigned numSamples = 200);
    void                       sweep() const;
    void                       testRayTraversedVoxels(const Ray3d& ray, std::vector<AABB>& traversedVoxels) const;
    void                       testRayTraversedVoxelsAccelerated(Ray3d& ray, std::vector<AABB>& traversedVoxels);
    void                       testRayTraversedVoxelsAcceleratedNeighbourhood(Ray3d& ray, std::vector<AABB>& traversedVoxels);
};
