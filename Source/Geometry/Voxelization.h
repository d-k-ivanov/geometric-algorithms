#pragma once

#include "Ray3d.h"
#include "TriangleModel.h"
#include "VoxelModel.h"

#include "Rendering/DrawVoxelization.h"

class Voxelization
{

private:
    glm::vec3                                          _size;
    std::vector<std::vector<std::vector<VoxelModel*>>> _voxels;
    double                                             _minX, _minY, _minZ;
    double                                             _maxX, _maxY, _maxZ;
    int                                                _numX, _numY, _numZ;

public:
    Voxelization();
    Voxelization(const Voxelization& voxel);
    Voxelization(double maxX, double maxY, double maxZ, double minX, double minY, double minZ, glm::vec3 size);
    Voxelization(TriangleModel* model, glm::vec3 size, int algorithm);
    virtual ~Voxelization() = default;

    VoxelModel*                                        getVoxel(double x, double y, double z);
    void                                               add(const Vect3d& data);
    void                                               sweep(const std::vector<Triangle3d>& triangles) const;
    static bool                                        compare(const std::pair<Vect3d, int>& v1, const std::pair<Vect3d, int>& v2);
    std::vector<VoxelModel*>                           getVoxelsInAABB(AABB& aabb);
    std::vector<std::vector<std::vector<VoxelModel*>>> getVoxels();

    bool rayTraversal(Ray3d& r, std::vector<VoxelModel*>& v);
    bool rayBoxIntersection(Ray3d& r, double& tMin, double& tMax, double t0, double t1) const;

    bool isInVoxel(VoxelModel* voxel, const Vect3d& vertice) const;

    GDSA::DrawVoxelization* getRenderingObject(bool outlineMode);

    void printData() const;

    double    getXMax() const { return _maxX; }
    double    getYMax() const { return _maxY; }
    double    getZMax() const { return _maxZ; }
    double    getXMin() const { return _minX; }
    double    getYMin() const { return _minY; }
    double    getZMin() const { return _minZ; }
    glm::vec3 getTam() const { return _size; }

protected:
    void flood();
    void recursiveFill(VoxelModel* v, int x, int y, int z);
};
