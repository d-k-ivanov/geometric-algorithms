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
    VoxelStatus  formatin;
    std::vector<Vect3d> puntos;
    Vect3d         centro, min, max;
    glm::vec3      tam;

public:
    Voxel();
    ~Voxel() = default;
    Voxel(const Voxel& v);
    Voxel(Vect3d puntoMin, glm::vec3 tamVoxel);
    void inserta(Vect3d punto);
    bool busca(Vect3d punto);
    bool borra(Vect3d punto);

    void setFormato(VoxelStatus formato) { formatin = formato; }

    Vect3d        getMin() { return this->min; }
    Vect3d        getMax() { return this->max; }
    VoxelStatus getFormato() { return this->formatin; }

    bool fuerzaBruta(Triangle3d triangulo);
    bool lineaBarrido(Triangle3d triangulo);
    bool aabb_tri(Triangle3d triangulo);

    bool planeBoxOverlap(const Vect3d& normal, const Vect3d& vert, glm::vec3 maxBox) const;
    bool triBoxOverlap(Vect3d centro, glm::vec3 tam, std::vector<Vect3d> vertices);

    bool axisTestX(double& a, double& b, float& fa, float& fb, float& p0, float& px, Vect3d& v0, Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad);
    bool axisTestY(double& a, double& b, float& fa, float& fb, float& p0, float& px, Vect3d& v0, Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad);
    bool axisTestZ(double& a, double& b, float& fa, float& fb, float& p0, float& px, Vect3d& v0, Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad);
};
