#include "Voxelization.h"

#include "Utils/ChronoUtilities.h"

#include <algorithm>
#include <map>
#include <set>

Voxelization::Voxelization()
    : _size(0)
    , _minX(0)
    , _minY(0)
    , _minZ(0)
    , _maxX(0)
    , _maxY(0)
    , _maxZ(0)
    , _numX(0)
    , _numY(0)
    , _numZ(0)
{
}

Voxelization::Voxelization(TriangleModel* model, glm::vec3 size, int algorithm)
{
    _size          = size;
    AABB modelAABB = model->getAABB();
    _maxX          = modelAABB.getMax().getX();
    _maxY          = modelAABB.getMax().getY();
    _maxZ          = modelAABB.getMax().getZ();
    _minX          = modelAABB.getMin().getX();
    _minY          = modelAABB.getMin().getY();
    _minZ          = modelAABB.getMin().getZ();
    _numX          = (_maxX - _minX) / _size[0] + 1;
    _numY          = (_maxY - _minY) / _size[1] + 1;
    _numZ          = (_maxZ - _minZ) / _size[2] + 1;

    double auxX, auxY, auxZ;
    auxX = _minX;
    auxY = _minY;
    auxZ = _minZ;
    for(int i = 0; i < _numX; i++)
    {
        std::vector<std::vector<Voxel*>> nivel1;
        for(int x = 0; x < _numY; x++)
        {
            std::vector<Voxel*> nivel2;
            for(int y = 0; y < _numZ; y++)
            {
                Vect3d puntoMinVoxel(auxX, auxY, auxZ);
                Voxel* insertado = new Voxel(puntoMinVoxel, _size);
                nivel2.push_back(insertado);
                auxZ += _size[2];
            }
            auxZ = _minZ;
            auxY += _size[1];
            nivel1.push_back(nivel2);
        }
        auxY = _minY;
        auxX += _size[0];
        _voxels.push_back(nivel1);
    }
    int                     aux1       = model->getFaces().size();
    std::vector<Triangle3d> triangulos = model->getFaces();
    switch(algorithm)
    {
        case 0:
            for(int i = 0; i < _numX; i++)
            {
                for(int x = 0; x < _numY; x++)
                {
                    for(int y = 0; y < _numZ; y++)
                    {
                        for(int j = 0; j < aux1; j++)
                        {
                            if(_voxels[i][x][y]->bruteForce(triangulos[j]))
                            {
                                _voxels[i][x][y]->setStatus(VoxelStatus::OCCUPIED);
                                j = aux1;
                            }
                        }
                    }
                }
            }
            break;
        case 1:
            lineSweep(model->getFaces());
            break;
        case 2:

            for(int j = 0; j < aux1; j++)
            {
                AABB                aabb_ti               = triangulos[j].getAABB();
                std::vector<Voxel*> voxeles_seleccionados = getVoxels(aabb_ti);
                for(int i = 0; i < voxeles_seleccionados.size(); i++)
                {
                    if(voxeles_seleccionados[i]->bruteForce(model->getFaces()[j]))
                    {
                        voxeles_seleccionados[i]->setStatus(VoxelStatus::OCCUPIED);
                    }
                }
            }
            break;
        default:
            lineSweep(model->getFaces());
            break;
    }
    this->flood();
}

Voxelization::Voxelization(double maxX, double maxY, double maxZ, double minX, double minY, double minZ, glm::vec3 size)
{
    _size = size;
    _maxX = maxX;
    _maxY = maxY;
    _maxZ = maxZ;
    _minX = minX;
    _minY = minY;
    _minZ = minZ;
    _numX = (_maxX - _minX) / _size[0] + 1;
    _numY = (_maxY - _minY) / _size[1] + 1;
    _numZ = (_maxZ - _minZ) / _size[2] + 1;

    double auxX, auxY, auxZ;
    auxX = _minX;
    auxY = _minY;
    auxZ = _minZ;
    for(int i = 0; i < _numX; i++)
    {
        std::vector<std::vector<Voxel*>> nivel1;
        for(int x = 0; x < _numY; x++)
        {
            std::vector<Voxel*> nivel2;
            for(int y = 0; y < _numZ; y++)
            {
                Vect3d puntoMinVoxel(auxX, auxY, auxZ);
                Voxel* insertado = new Voxel(puntoMinVoxel, _size);
                nivel2.push_back(insertado);
                auxZ += _size[2];
            }
            auxZ = _minZ;
            auxY += _size[1];
            nivel1.push_back(nivel2);
        }
        auxY = _minY;
        auxX += _size[0];
        _voxels.push_back(nivel1);
    }
}

Voxelization::Voxelization(const Voxelization& voxel)
    : _size(voxel._size)
    , _minX(voxel._minX)
    , _minY(voxel._minY)
    , _minZ(voxel._minZ)
    , _maxX(voxel._maxX)
    , _maxY(voxel._maxY)
    , _maxZ(voxel._maxZ)
    , _numX(voxel._numX)
    , _numY(voxel._numY)
    , _numZ(voxel._numZ)
{
}

Voxel* Voxelization::getVoxel(double x, double y, double z)
{
    const int i   = static_cast<int>(glm::abs(x / _size[0])) % _numX;
    const int j   = static_cast<int>(glm::abs(y / _size[1])) % _numY;
    const int k   = static_cast<int>(glm::abs(z / _size[2])) % _numZ;
    Voxel*    res = _voxels[i][j][k];

    return res;
}

void Voxelization::add(const Vect3d& data)
{
    const double x = data.getX();
    const double y = data.getY();
    const double z = data.getZ();

    Voxel* v = getVoxel(x, y, z);
    v->add(data);
}

bool Voxelization::compare(const std::pair<Vect3d, int>& v1, const std::pair<Vect3d, int>& v2)
{
    return v1.first.getY() < v2.first.getY();
}

void Voxelization::lineSweep(const std::vector<Triangle3d>& triangles) const
{

    std::vector<std::pair<Vect3d, int>> vertices;

    for(int i = 0; i < triangles.size(); i++)
    {
        Triangle3d triangle = triangles[i];
        vertices.emplace_back(triangle.getA(), i);
        vertices.emplace_back(triangle.getB(), i);
        vertices.emplace_back(triangle.getC(), i);
    }

    std::sort(vertices.begin(), vertices.end(), &compare);

    std::map<int, int>           triangulos_linea;
    std::vector<Voxel*>          voxeles_linea;
    std::map<int, int>::iterator it;
    int                          indice = 0;
    for(int i = 0; i < _numY; i++)
    {
        const double nivelMaxY = this->_voxels[0][i][0]->getMax().getY();
        const double nivelMinY = this->_voxels[0][i][0]->getMin().getY();
        for(int x = 0; x < _numX; x++)
        {
            for(int y = 0; y < _numZ; y++)
            {
                voxeles_linea.push_back(this->_voxels[x][i][y]);
            }
        }
        for(int v = 0; v < vertices.size(); v++)
        {
            if((vertices[v].first.getY() > nivelMinY || BasicGeometry::equal(vertices[v].first.getY(), nivelMinY))
               && (vertices[v].first.getY() < nivelMaxY || BasicGeometry::equal(vertices[v].first.getY(), nivelMaxY)))
            {
                it = triangulos_linea.find(vertices[v].second);
                if(it != triangulos_linea.end())
                {
                    it->second += 1;
                }
                else
                {
                    triangulos_linea.insert(std::pair(vertices[v].second, 0));
                }
            }
            else if(vertices[v].first.getY() > nivelMaxY)
            {
                break;
            }
        }
        for(int j = 0; j < voxeles_linea.size(); j++)
        {
            for(it = triangulos_linea.begin(); it != triangulos_linea.end(); ++it)
            {
                if(voxeles_linea[j]->bruteForce(triangles[it->first]))
                {
                    voxeles_linea[j]->setStatus(VoxelStatus::OCCUPIED);
                    break;
                }
            }
        }
        it = triangulos_linea.begin();
        while(it != triangulos_linea.end())
        {
            if(it->second == 2)
            {
                it = triangulos_linea.erase(it);
            }
            else
            {
                it++;
            }
        }
        voxeles_linea.clear();
    }
}

std::vector<Voxel*> Voxelization::getVoxels(AABB aabb)
{

    std::vector<std::pair<Voxel*, glm::vec3>> solucion;

    Vect3d e1(aabb.getMin());
    Vect3d e2(aabb.getMax());
    Vect3d e3(aabb.getMin().getX(), aabb.getMin().getY(), aabb.getMax().getZ());
    Vect3d e4(aabb.getMax().getX(), aabb.getMin().getY(), aabb.getMax().getZ());
    Vect3d e5(aabb.getMin().getX(), aabb.getMax().getY(), aabb.getMax().getZ());
    Vect3d e6(aabb.getMax().getX(), aabb.getMin().getY(), aabb.getMin().getZ());
    Vect3d e7(aabb.getMin().getX(), aabb.getMax().getY(), aabb.getMin().getZ());
    Vect3d e8(aabb.getMax().getX(), aabb.getMax().getY(), aabb.getMin().getZ());

    for(int i = 0; i < _numX; i++)
    {
        for(int x = 0; x < _numY; x++)
        {
            for(int y = 0; y < _numZ; y++)
            {

                if(_voxels[i][x][y]->getStatus() != VoxelStatus::OCCUPIED)
                {
                    if(isInVoxel(_voxels[i][x][y], e1) || isInVoxel(_voxels[i][x][y], e2) || isInVoxel(_voxels[i][x][y], e3) || isInVoxel(_voxels[i][x][y], e4)
                       || isInVoxel(_voxels[i][x][y], e5) || isInVoxel(_voxels[i][x][y], e6) || isInVoxel(_voxels[i][x][y], e7) || isInVoxel(_voxels[i][x][y], e8))
                    {
                        glm::vec3 aux(i, x, y);
                        solucion.push_back(std::pair(_voxels[i][x][y], aux));
                    }
                }
            }
        }
    }
    std::vector<Voxel*> res;
    for(int i = 0; i < solucion.size(); i++)
    {
        res.push_back(solucion[i].first);
    }

    return res;
}

std::vector<std::vector<std::vector<Voxel*>>> Voxelization::getVoxels()
{
    return _voxels;
}

bool Voxelization::isInVoxel(Voxel* voxel, const Vect3d& vertice) const
{
    return vertice.getX() >= voxel->getMin().getX()
        && vertice.getX() <= voxel->getMax().getX()
        && vertice.getY() >= voxel->getMin().getY()
        && vertice.getY() <= voxel->getMax().getY()
        && vertice.getZ() >= voxel->getMin().getZ()
        && vertice.getZ() <= voxel->getMax().getZ();
}

void Voxelization::flood()
{
    int centralX, centralY, centralZ;
    centralX = _numX / 2;
    centralY = _numY / 2;
    centralZ = _numZ / 2;

    if(this->_voxels[centralX][centralY][centralZ]->getStatus() == VoxelStatus::OCCUPIED)
    {
        while(this->_voxels[centralX][centralY][centralZ]->getStatus() == VoxelStatus::OCCUPIED)
        {
            centralX++;
        }
    }
    recursive(this->_voxels[centralX][centralY][centralZ], centralX, centralY, centralZ);
    for(int i = 0; i < _numX; i++)
    {
        for(int x = 0; x < _numY; x++)
        {
            for(int y = 0; y < _numZ; y++)
            {
                if(this->_voxels[i][x][y]->getStatus() == VoxelStatus::NP)
                {
                    this->_voxels[i][x][y]->setStatus(VoxelStatus::OUTER);
                }
            }
        }
    }
}

void Voxelization::recursive(Voxel* v, int x, int y, int z)
{
    if(v->getStatus() == VoxelStatus::OCCUPIED || v->getStatus() == VoxelStatus::INNER)
    {
        return;
    }
    v->setStatus(VoxelStatus::INNER);

    if(x + 1 < _numX)
    {
        recursive(this->_voxels[x + 1][y][z], x + 1, y, z);
    }
    if(x - 1 > 0)
    {
        recursive(this->_voxels[x - 1][y][z], x - 1, y, z);
    }
    if(y + 1 < _numY)
    {
        recursive(this->_voxels[x][y + 1][z], x, y + 1, z);
    }
    if(y - 1 > 0)
    {
        recursive(this->_voxels[x][y - 1][z], x, y - 1, z);
    }
    if(z + 1 < _numZ)
    {
        recursive(this->_voxels[x][y][z + 1], x, y, z + 1);
    }
    if(z - 1 > 0)
    {
        recursive(this->_voxels[x][y][z - 1], x, y, z - 1);
    }
}

AlgGeom::DrawVoxelization* Voxelization::getRenderingObject(bool occupied)
{
    VoxelStatus status;
    if(occupied)
        status = VoxelStatus::OCCUPIED;
    else
        status = VoxelStatus::INNER;

    std::vector<glm::vec3> vector;
    const Vect3d           displace(_size[0] / 2, _size[1] / 2, _size[2] / 2);
    for(int i = 0; i < _numX; i++)
    {
        for(int x = 0; x < _numY; x++)
        {
            for(int y = 0; y < _numZ; y++)
            {
                if(this->getVoxels()[i][x][y]->getStatus() == status)
                {
                    Vect3d centro = this->getVoxels()[i][x][y]->getMin().add(displace);
                    vector.push_back(glm::vec3(centro.getX(), centro.getY(), centro.getZ()));
                }
            }
        }
    }
    glm::vec3* positions = new glm::vec3[vector.size()];
    glm::vec3* color     = new glm::vec3[vector.size()];
    for(int i = 0; i < vector.size(); i++)
    {
        positions[i] = vector[i];
        color[i]     = glm::vec3(0, 0, 0);
    }

    AlgGeom::DrawVoxelization* voxelization = new AlgGeom::DrawVoxelization(positions, vector.size(), _size, color);

    // delete positions;
    // delete color;
    return voxelization;
}

bool Voxelization::rayTraversal(Ray3d& r, std::vector<Voxel*>& v)
{
    double     tMin;
    double     tMax;
    const bool ray_intersects_grid = rayBoxIntersection(r, tMin, tMax, 0, 1);
    if(!ray_intersects_grid)
        return 0;

    tMin             = BasicGeometry::max2(tMin, 0);
    tMax             = BasicGeometry::max2(tMax, 1);
    Vect3d aux       = r.getOrigin();
    Vect3d ray_start = r.getDirection().scalarMul(tMin).add(aux);
    Vect3d ray_end   = r.getDirection().scalarMul(tMax).add(aux);

    size_t       current_X_index = BasicGeometry::max2(1, std::ceil((ray_start.getX() - this->getXMin()) / this->_size.x));
    const size_t end_X_index     = BasicGeometry::max2(1, std::ceil((ray_end.getX() - this->getXMin()) / this->_size.x));
    int          stepX;
    double       tDeltaX;
    double       tMaxX;
    if(r.getDirection().getX() > 0.0)
    {
        stepX   = 1;
        tDeltaX = this->_size[0] / r.getDirection().getX();
        tMaxX   = tMin + (this->getXMin() + current_X_index * this->_size.x - ray_start.getX()) / r.getDirection().getX();
    }
    else if(r.getDirection().getX() < 0.0)
    {
        stepX                         = -1;
        tDeltaX                       = this->_size[0] / -r.getDirection().getX();
        const size_t previous_X_index = current_X_index - 1;
        tMaxX                         = tMin + (this->getXMin() + previous_X_index * this->_size.x - ray_start.getX()) / r.getDirection().getX();
    }
    else
    {
        stepX   = 0;
        tDeltaX = tMax;
        tMaxX   = tMax;
    }

    size_t       current_Y_index = BasicGeometry::max2(1, std::ceil((ray_start.getY() - this->getYMin()) / this->_size.y));
    const size_t end_Y_index     = BasicGeometry::max2(1, std::ceil((ray_end.getY() - this->getYMin()) / this->_size.y));
    int          stepY;
    double       tDeltaY;
    double       tMaxY;
    if(r.getDirection().getY() > 0.0)
    {
        stepY   = 1;
        tDeltaY = this->_size[1] / r.getDirection().getY();
        tMaxY   = tMin + (this->getYMin() + current_Y_index * this->_size.y - ray_start.getY()) / r.getDirection().getY();
    }
    else if(r.getDirection().getY() < 0.0)
    {
        stepY                         = -1;
        tDeltaY                       = this->_size[1] / -r.getDirection().getY();
        const size_t previous_Y_index = current_Y_index - 1;
        tMaxY                         = tMin + (this->getYMin() + previous_Y_index * this->_size.y - ray_start.getY()) / r.getDirection().getY();
    }
    else
    {
        stepY   = 0;
        tDeltaY = tMax;
        tMaxY   = tMax;
    }

    size_t       current_Z_index = BasicGeometry::max2(1, std::ceil((ray_start.getZ() - this->getZMin()) / this->_size.z));
    const size_t end_Z_index     = BasicGeometry::max2(1, std::ceil((ray_end.getZ() - this->getZMin()) / this->_size.z));
    int          stepZ;
    double       tDeltaZ;
    double       tMaxZ;
    if(r.getDirection().getZ() > 0.0)
    {
        stepZ   = 1;
        tDeltaZ = this->_size[2] / r.getDirection().getZ();
        tMaxZ   = tMin + (this->getZMin() + current_Z_index * this->_size.z - ray_start.getZ()) / r.getDirection().getZ();
    }
    else if(r.getDirection().getZ() < 0.0)
    {
        stepZ                         = -1;
        tDeltaZ                       = this->_size[2] / -r.getDirection().getZ();
        const size_t previous_Z_index = current_Z_index - 1;
        tMaxZ                         = tMin + (this->getZMin() + previous_Z_index * this->_size.z - ray_start.getZ()) / r.getDirection().getZ();
    }
    else
    {
        stepZ   = 0;
        tDeltaZ = tMax;
        tMaxZ   = tMax;
    }
    while(glm::all(glm::greaterThanEqual(glm::vec3(current_X_index, current_Y_index, current_Z_index), glm::vec3(1, 1, 1))) && glm::all(glm::lessThanEqual(glm::vec3(current_X_index, current_Y_index, current_Z_index), glm::vec3(_numX, _numY, _numZ))))
    {
        /*while (current_X_index != end_X_index && current_Y_index != end_Y_index && current_Z_index != end_Z_index) {*/
        v.push_back(this->_voxels[current_X_index - 1][current_Y_index - 1][current_Z_index - 1]);
        if(tMaxX < tMaxY && tMaxX < tMaxZ)
        {
            // X-axis traversal.
            current_X_index += stepX;
            tMaxX += tDeltaX;
        }
        else if(tMaxY < tMaxZ)
        {
            // Y-axis traversal.
            current_Y_index += stepY;
            tMaxY += tDeltaY;
        }
        else
        {
            // Z-axis traversal.
            current_Z_index += stepZ;
            tMaxZ += tDeltaZ;
        }
    }
    if(v.size() == 0)
        return false;
    return true;
}

bool Voxelization::rayBoxIntersection(Ray3d& r, double& tMin, double& tMax, double t0, double t1)
{
    double       tYMin, tYMax, tZMin, tZMax;
    const double x_inv_dir = 1 / r.getDirection().getX();
    if(x_inv_dir >= 0)
    {
        tMin = (this->getXMin() - r.getOrigin().getX()) * x_inv_dir;
        tMax = (this->getXMax() - r.getOrigin().getX()) * x_inv_dir;
    }
    else
    {
        tMin = (this->getXMax() - r.getOrigin().getX()) * x_inv_dir;
        tMax = (this->getXMin() - r.getOrigin().getX()) * x_inv_dir;
    }

    const double y_inv_dir = 1 / r.getDirection().getY();
    if(y_inv_dir >= 0)
    {
        tYMin = (this->getYMin() - r.getOrigin().getY()) * y_inv_dir;
        tYMax = (this->getYMax() - r.getOrigin().getY()) * y_inv_dir;
    }
    else
    {
        tYMin = (this->getYMax() - r.getOrigin().getY()) * y_inv_dir;
        tYMax = (this->getYMin() - r.getOrigin().getY()) * y_inv_dir;
    }

    if(tMin > tYMax || tYMin > tMax)
        return false;
    if(tYMin > tMin)
        tMin = tYMin;
    if(tYMax < tMax)
        tMax = tYMax;

    const double z_inv_dir = 1 / r.getDirection().getZ();
    if(z_inv_dir >= 0)
    {
        tZMin = (this->getZMin() - r.getOrigin().getZ()) * z_inv_dir;
        tZMax = (this->getZMax() - r.getOrigin().getZ()) * z_inv_dir;
    }
    else
    {
        tZMin = (this->getZMax() - r.getOrigin().getZ()) * z_inv_dir;
        tZMax = (this->getZMin() - r.getOrigin().getZ()) * z_inv_dir;
    }

    if(tMin > tZMax || tZMin > tMax)
        return false;
    if(tZMin > tMin)
        tMin = tZMin;
    if(tZMax < tMax)
        tMax = tZMax;
    return tMin < t1 && tMax > t0;
}

Voxelization* Voxelization::AND(Voxelization& voxelization)
{

    Voxelization* res;
    glm::vec3     num_voxeles;

    glm::vec3 tamNuevo(BasicGeometry::max2(this->getXMax() - this->getXMin(), voxelization.getXMax() - voxelization.getXMin()), BasicGeometry::max2(this->getYMax() - this->getYMin(), voxelization.getYMax() - voxelization.getYMin()), BasicGeometry::max2(this->getZMax() - this->getZMin(), voxelization.getZMax() - voxelization.getZMin()));

    res = new Voxelization(BasicGeometry::max2(this->getXMax(), voxelization.getXMax()), BasicGeometry::max2(this->getYMax(), voxelization.getYMax()),
                           BasicGeometry::max2(this->getZMax(), voxelization.getZMax()), BasicGeometry::min2(this->getXMin(), voxelization.getXMin()),
                           BasicGeometry::min2(this->getYMin(), voxelization.getYMin()), BasicGeometry::min2(this->getZMin(), voxelization.getZMin()), glm::vec3(_size.x));

    num_voxeles = glm::vec3(res->_numX, res->_numY, res->_numZ);
    int xV, yV, zV;
    int x2, y2, z2;
    for(int i = 0; i < num_voxeles.x; i++)
    {
        for(int y = 0; y < num_voxeles.y; y++)
        {
            for(int z = 0; z < num_voxeles.z; z++)
            {
                if(checkMembership(res->getVoxels()[i][y][z], this, xV, yV, zV))
                {
                    if(checkMembership(res->getVoxels()[i][y][z], &voxelization, x2, y2, z2))
                    {
                        if(this->getVoxels()[xV][yV][zV]->getStatus() != VoxelStatus::OUTER && voxelization.getVoxels()[x2][y2][z2]->getStatus() != VoxelStatus::OUTER)
                            res->getVoxels()[i][y][z]->setStatus(VoxelStatus::INNER);
                        else
                            res->getVoxels()[i][y][z]->setStatus(VoxelStatus::OUTER);
                    }
                    else
                    {
                        res->getVoxels()[i][y][z]->setStatus(VoxelStatus::OUTER);
                    }
                }
                else
                {
                    res->getVoxels()[i][y][z]->setStatus(VoxelStatus::OUTER);
                }
            }
        }
    }

    return res;
}

Voxelization* Voxelization::OR(Voxelization& voxelization)
{

    Voxelization* res;
    glm::vec3     num_voxeles;

    glm::vec3 tamNuevo(BasicGeometry::max2(this->getXMax() - this->getXMin(), voxelization.getXMax() - voxelization.getXMin()), BasicGeometry::max2(this->getYMax() - this->getYMin(), voxelization.getYMax() - voxelization.getYMin()), BasicGeometry::max2(this->getZMax() - this->getZMin(), voxelization.getZMax() - voxelization.getZMin()));

    res = new Voxelization(BasicGeometry::max2(this->getXMax(), voxelization.getXMax()), BasicGeometry::max2(this->getYMax(), voxelization.getYMax()),
                           BasicGeometry::max2(this->getZMax(), voxelization.getZMax()), BasicGeometry::min2(this->getXMin(), voxelization.getXMin()),
                           BasicGeometry::min2(this->getYMin(), voxelization.getYMin()), BasicGeometry::min2(this->getZMin(), voxelization.getZMin()), glm::vec3(_size.x));

    num_voxeles = glm::vec3(res->_numX, res->_numY, res->_numZ);

    int xV, yV, zV;
    int x2, y2, z2;
    for(int i = 0; i < num_voxeles.x; i++)
    {
        for(int y = 0; y < num_voxeles.y; y++)
        {
            for(int z = 0; z < num_voxeles.z; z++)
            {
                if(checkMembership(res->getVoxels()[i][y][z], this, xV, yV, zV))
                {
                    if(checkMembership(res->getVoxels()[i][y][z], &voxelization, x2, y2, z2))
                    {
                        if(this->getVoxels()[xV][yV][zV]->getStatus() != VoxelStatus::OUTER || voxelization.getVoxels()[x2][y2][z2]->getStatus() != VoxelStatus::OUTER)
                            res->getVoxels()[i][y][z]->setStatus(VoxelStatus::INNER);
                        else
                            res->getVoxels()[i][y][z]->setStatus(VoxelStatus::OUTER);
                    }
                    else
                    {
                        if(this->getVoxels()[xV][yV][zV]->getStatus() != VoxelStatus::OUTER)
                            res->getVoxels()[i][y][z]->setStatus(VoxelStatus::INNER);
                        else
                            res->getVoxels()[i][y][z]->setStatus(VoxelStatus::OUTER);
                    }
                }
                else
                {
                    if(checkMembership(res->getVoxels()[i][y][z], &voxelization, x2, y2, z2))
                    {
                        if(voxelization.getVoxels()[x2][y2][z2]->getStatus() != VoxelStatus::OUTER)
                            res->getVoxels()[i][y][z]->setStatus(VoxelStatus::INNER);
                        else
                            res->getVoxels()[i][y][z]->setStatus(VoxelStatus::OUTER);
                    }
                    else
                    {
                        res->getVoxels()[i][y][z]->setStatus(VoxelStatus::OUTER);
                    }
                }
            }
        }
    }

    return res;
}

Voxelization* Voxelization::XOR(Voxelization& voxelization)
{

    Voxelization* res;
    glm::vec3     num_voxeles;

    glm::vec3 tamNuevo(BasicGeometry::max2(this->getXMax() - this->getXMin(), voxelization.getXMax() - voxelization.getXMin()), BasicGeometry::max2(this->getYMax() - this->getYMin(), voxelization.getYMax() - voxelization.getYMin()), BasicGeometry::max2(this->getZMax() - this->getZMin(), voxelization.getZMax() - voxelization.getZMin()));

    res = new Voxelization(BasicGeometry::max2(this->getXMax(), voxelization.getXMax()), BasicGeometry::max2(this->getYMax(), voxelization.getYMax()),
                           BasicGeometry::max2(this->getZMax(), voxelization.getZMax()), BasicGeometry::min2(this->getXMin(), voxelization.getXMin()),
                           BasicGeometry::min2(this->getYMin(), voxelization.getYMin()), BasicGeometry::min2(this->getZMin(), voxelization.getZMin()), glm::vec3(_size.x));

    num_voxeles = glm::vec3(res->_numX, res->_numY, res->_numZ);

    int xV, yV, zV;
    int x2, y2, z2;
    for(int i = 0; i < num_voxeles.x; i++)
    {
        for(int y = 0; y < num_voxeles.y; y++)
        {
            for(int z = 0; z < num_voxeles.z; z++)
            {
                if(checkMembership(res->getVoxels()[i][y][z], this, xV, yV, zV))
                {
                    if(checkMembership(res->getVoxels()[i][y][z], &voxelization, x2, y2, z2))
                    {
                        if(this->getVoxels()[xV][yV][zV]->getStatus() != VoxelStatus::OUTER && voxelization.getVoxels()[x2][y2][z2]->getStatus() == VoxelStatus::OUTER)
                            res->getVoxels()[i][y][z]->setStatus(VoxelStatus::INNER);
                        else
                            res->getVoxels()[i][y][z]->setStatus(VoxelStatus::OUTER);
                    }
                    else
                    {
                        if(this->getVoxels()[xV][yV][zV]->getStatus() != VoxelStatus::OUTER)
                            res->getVoxels()[i][y][z]->setStatus(VoxelStatus::INNER);
                        else
                            res->getVoxels()[i][y][z]->setStatus(VoxelStatus::OUTER);
                    }
                }
                else
                {
                    res->getVoxels()[i][y][z]->setStatus(VoxelStatus::OUTER);
                }
            }
        }
    }

    return res;
}

bool Voxelization::checkMembership(Voxel* voxel, const Voxelization* voxelization, int& x, int& y, int& z) const
{
    if((BasicGeometry::equal(voxel->getMin().getX(), voxelization->getXMin()) || voxel->getMin().getX() > voxelization->getXMin())
       && (BasicGeometry::equal(voxel->getMax().getX(), voxelization->getXMax()) || voxel->getMax().getX() < voxelization->getXMax()))
    {

        if((BasicGeometry::equal(voxel->getMin().getY(), voxelization->getYMin()) || voxel->getMin().getY() > voxelization->getYMin())
           && (BasicGeometry::equal(voxel->getMax().getY(), voxelization->getYMax()) || voxel->getMax().getY() < voxelization->getYMax()))
        {

            if((BasicGeometry::equal(voxel->getMin().getZ(), voxelization->getZMin()) || voxel->getMin().getZ() > voxelization->getZMin())
               && (BasicGeometry::equal(voxel->getMax().getZ(), voxelization->getZMax()) || voxel->getMax().getZ() < voxelization->getZMax()))
            {

                x = static_cast<int>((voxel->getMin().getX() - voxelization->getXMin()) / voxelization->getTam().x);
                y = static_cast<int>((voxel->getMin().getY() - voxelization->getYMin()) / voxelization->getTam().y);
                z = static_cast<int>((voxel->getMin().getZ() - voxelization->getZMin()) / voxelization->getTam().z);

                return true;
            }
        }
    }
    return false;
}
