#pragma once

#include "Triangle3d.h"
#include "Vect3d.h"

#include "iostream"
#include "vector"

enum VoxelStatus
{
    GRIS,
    BLANCO,
    NEGRO,
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
    Voxel(const Voxel& v);
    Voxel(Vect3d minPoint, glm::vec3 tamVoxel);
    void inserta(Vect3d punto);
    bool busca(Vect3d punto);
    bool borra(Vect3d punto);

    void setFormato(VoxelStatus formato) { _status = formato; }

    Vect3d      getMin() { return this->_min; }
    Vect3d      getMax() { return this->_max; }
    VoxelStatus getFormato() { return this->_status; }

    bool fuerzaBruta(Triangle3d triangulo);
    bool lineaBarrido(Triangle3d triangulo);
    bool aabb_tri(Triangle3d triangulo);

    bool planeBoxOverlap(const Vect3d& normal, const Vect3d& vert, glm::vec3 maxBox) const;
    bool triBoxOverlap(Vect3d centro, glm::vec3 tam, std::vector<Vect3d> vertices);

    bool axisTestX(double& a, double& b, float& fa, float& fb, float& p0, float& px, Vect3d& v0, Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad);
    bool axisTestY(double& a, double& b, float& fa, float& fb, float& p0, float& px, Vect3d& v0, Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad);
    bool axisTestZ(double& a, double& b, float& fa, float& fb, float& p0, float& px, Vect3d& v0, Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad);
};
