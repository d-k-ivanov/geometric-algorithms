#pragma once

#include "Ray3d.h"
#include "TriangleModel.h"
#include "VoxelModel.h"

#include "Rendering/DrawVoxelization.h"

class Voxelization
{

private:
    glm::vec3                                     _size;
    std::vector<std::vector<std::vector<Voxel*>>> _voxels;
    double                                        _minX, _minY, _minZ;
    double                                        _maxX, _maxY, _maxZ;
    int                                           _numX, _numY, _numZ;

    bool comprobarPertenencia(Voxel* v, Voxelization* voxelizacion, int& x, int& y, int& z);

public:
    Voxelization();
    Voxelization(const Voxelization& voxel);
    Voxelization(double _xmax, double _ymax, double _zmax, double _xmin, double _ymin, double _zmin, glm::vec3 _tam);
    Voxelization(TriangleModel* model, glm::vec3 size, int algorithm);
    virtual ~Voxelization();

    Voxel*                                        getVoxel(double x, double y, double z);
    void                                          add(Vect3d data);
    void                                          lineSweep(std::vector<Triangle3d> triangulos);
    static bool                                   comp(const std::pair<Vect3d, int>& v1, const std::pair<Vect3d, int>& v2);
    std::vector<Voxel*>                           getVoxels(AABB aabb_ti);
    std::vector<std::vector<std::vector<Voxel*>>> getVoxels() { return _voxels; }

    bool rayTraversal(Ray3d& r, std::vector<Voxel*>& v);
    bool rayBoxIntersection(Ray3d& r, double& tMin, double& tMax, double t0, double t1);

    bool isInVoxel(Voxel* v, Vect3d vertice);

    AlgGeom::DrawVoxelization* getRenderingObject(bool gris);

    double    getXMax() const { return _maxX; }
    double    getYMax() const { return _maxY; }
    double    getZMax() const { return _maxZ; }
    double    getXMin() const { return _minX; }
    double    getYMin() const { return _minY; }
    double    getZMin() const { return _minZ; }
    glm::vec3 getTam() const { return _size; }

    Voxelization* AND(Voxelization& v);
    Voxelization* OR(Voxelization& v);
    Voxelization* XOR(Voxelization& v);

protected:
    void flood();
    void recursivo(Voxel* v, int x, int y, int z);
};
