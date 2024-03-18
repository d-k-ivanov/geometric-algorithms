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
    _numX          = static_cast<int>((_maxX - _minX) / _size[0] + 1);
    _numY          = static_cast<int>((_maxY - _minY) / _size[1] + 1);
    _numZ          = static_cast<int>((_maxZ - _minZ) / _size[2] + 1);

    double tMinX = _minX;
    double tMinY = _minY;
    double tMinZ = _minZ;
    for(int i = 0; i < _numX; i++)
    {
        std::vector<std::vector<VoxelModel*>> nivel1;
        for(int x = 0; x < _numY; x++)
        {
            std::vector<VoxelModel*> nivel2;
            for(int y = 0; y < _numZ; y++)
            {
                Vect3d puntoMinVoxel(tMinX, tMinY, tMinZ);
                VoxelModel* insertado = new VoxelModel(puntoMinVoxel, _size);
                nivel2.push_back(insertado);
                tMinZ += _size[2];
            }
            tMinZ = _minZ;
            tMinY += _size[1];
            nivel1.push_back(nivel2);
        }
        tMinY = _minY;
        tMinX += _size[0];
        _voxels.push_back(nivel1);
    }

    std::vector<Triangle3d> triangles = model->getFaces();

    size_t fSize = model->getFaces().size();
    switch(algorithm)
    {
        case 0:
            for(int i = 0; i < _numX; i++)
            {
                for(int x = 0; x < _numY; x++)
                {
                    for(int y = 0; y < _numZ; y++)
                    {
                        for(size_t j = 0; j < fSize; j++)
                        {
                            if(_voxels[i][x][y]->bruteForce(triangles[j]))
                            {
                                _voxels[i][x][y]->setStatus(VoxelStatus::OCCUPIED);
                                j = fSize;
                            }
                        }
                    }
                }
            }
            break;
        case 1:
            sweep(model->getFaces());
            break;
        case 2:

            for(size_t j = 0; j < fSize; j++)
            {
                AABB aabb = triangles[j].getAABB();

                for(auto& secondaryVoxel : getVoxelsInAABB(aabb))
                {
                    if(secondaryVoxel->bruteForce(model->getFaces()[j]))
                    {
                        secondaryVoxel->setStatus(VoxelStatus::OCCUPIED);
                    }
                }
            }
            break;
        default:
            sweep(model->getFaces());
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

    double tMinX = _minX;
    double tMinY = _minY;
    double tMinZ = _minZ;
    for(int i = 0; i < _numX; i++)
    {
        std::vector<std::vector<VoxelModel*>> level1;
        for(int x = 0; x < _numY; x++)
        {
            std::vector<VoxelModel*> level2;
            for(int y = 0; y < _numZ; y++)
            {
                const Vect3d voxel(tMinX, tMinY, tMinZ);
                VoxelModel*       voxelInserted = new VoxelModel(voxel, _size);
                level2.push_back(voxelInserted);
                tMinZ += _size[2];
            }
            tMinZ = _minZ;
            tMinY += _size[1];
            level1.push_back(level2);
        }
        tMinY = _minY;
        tMinX += _size[0];
        _voxels.push_back(level1);
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

VoxelModel* Voxelization::getVoxel(double x, double y, double z)
{
    const int i   = static_cast<int>(glm::abs(x / _size[0])) % _numX;
    const int j   = static_cast<int>(glm::abs(y / _size[1])) % _numY;
    const int k   = static_cast<int>(glm::abs(z / _size[2])) % _numZ;
    VoxelModel*    res = _voxels[i][j][k];

    return res;
}

void Voxelization::add(const Vect3d& data)
{
    const double x = data.getX();
    const double y = data.getY();
    const double z = data.getZ();

    VoxelModel* v = getVoxel(x, y, z);
    v->add(data);
}

bool Voxelization::compare(const std::pair<Vect3d, int>& v1, const std::pair<Vect3d, int>& v2)
{
    return v1.first.getY() < v2.first.getY();
}

void Voxelization::sweep(const std::vector<Triangle3d>& triangles) const
{
    std::vector<std::pair<Vect3d, int>> vertices;

    for(int i = 0; i < static_cast<int>(triangles.size()); i++)
    {
        Triangle3d triangle = triangles[i];
        vertices.emplace_back(triangle.getA(), i);
        vertices.emplace_back(triangle.getB(), i);
        vertices.emplace_back(triangle.getC(), i);
    }

    std::ranges::sort(vertices, &compare);

    std::map<int, int>           triangularPairs;
    std::vector<VoxelModel*>          voxels;
    std::map<int, int>::iterator it;
    for(int i = 0; i < _numY; i++)
    {
        const double levelMaxY = this->_voxels[0][i][0]->getMax().getY();
        const double levelMinY = this->_voxels[0][i][0]->getMin().getY();

        for(int x = 0; x < _numX; x++)
        {
            for(int y = 0; y < _numZ; y++)
            {
                voxels.push_back(this->_voxels[x][i][y]);
            }
        }

        for(auto& vertex : vertices)
        {
            if((vertex.first.getY() > levelMinY || BasicGeometry::equal(vertex.first.getY(), levelMinY))
               && (vertex.first.getY() < levelMaxY || BasicGeometry::equal(vertex.first.getY(), levelMaxY)))
            {
                it = triangularPairs.find(vertex.second);
                if(it != triangularPairs.end())
                {
                    it->second += 1;
                }
                else
                {
                    triangularPairs.insert(std::pair(vertex.second, 0));
                }
            }
            else if(vertex.first.getY() > levelMaxY)
            {
                break;
            }
        }

        for(const auto& voxel : voxels)
        {
            for(it = triangularPairs.begin(); it != triangularPairs.end(); ++it)
            {
                if(voxel->bruteForce(triangles[it->first]))
                {
                    voxel->setStatus(VoxelStatus::OCCUPIED);
                    break;
                }
            }
        }

        it = triangularPairs.begin();
        while(it != triangularPairs.end())
        {
            if(it->second == 2)
            {
                it = triangularPairs.erase(it);
            }
            else
            {
                ++it;
            }
        }
        voxels.clear();
    }
}

std::vector<VoxelModel*> Voxelization::getVoxelsInAABB(AABB& aabb)
{

    std::vector<VoxelModel*> result;

    const Vect3d corner1(aabb.getMin());
    const Vect3d corner2(aabb.getMax());
    const Vect3d corner3(aabb.getMin().getX(), aabb.getMin().getY(), aabb.getMax().getZ());
    const Vect3d corner4(aabb.getMax().getX(), aabb.getMin().getY(), aabb.getMax().getZ());
    const Vect3d corner5(aabb.getMin().getX(), aabb.getMax().getY(), aabb.getMax().getZ());
    const Vect3d corner6(aabb.getMax().getX(), aabb.getMin().getY(), aabb.getMin().getZ());
    const Vect3d corner7(aabb.getMin().getX(), aabb.getMax().getY(), aabb.getMin().getZ());
    const Vect3d corner8(aabb.getMax().getX(), aabb.getMax().getY(), aabb.getMin().getZ());

    for(int i = 0; i < _numX; i++)
    {
        for(int x = 0; x < _numY; x++)
        {
            for(int y = 0; y < _numZ; y++)
            {

                if(_voxels[i][x][y]->getStatus() != VoxelStatus::OCCUPIED)
                {
                    if(isInVoxel(_voxels[i][x][y], corner1) || isInVoxel(_voxels[i][x][y], corner2)
                       || isInVoxel(_voxels[i][x][y], corner3) || isInVoxel(_voxels[i][x][y], corner4)
                       || isInVoxel(_voxels[i][x][y], corner5) || isInVoxel(_voxels[i][x][y], corner6)
                       || isInVoxel(_voxels[i][x][y], corner7) || isInVoxel(_voxels[i][x][y], corner8))
                    {
                        result.emplace_back(_voxels[i][x][y]);
                    }
                }
            }
        }
    }

    return result;
}

std::vector<std::vector<std::vector<VoxelModel*>>> Voxelization::getVoxels()
{
    return _voxels;
}

bool Voxelization::isInVoxel(VoxelModel* voxel, const Vect3d& vertice) const
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
    int       centerX = _numX / 2;
    const int centerY = _numY / 2;
    const int centerZ = _numZ / 2;

    if(this->_voxels[centerX][centerY][centerZ]->getStatus() == VoxelStatus::OCCUPIED)
    {
        while(this->_voxels[centerX][centerY][centerZ]->getStatus() == VoxelStatus::OCCUPIED)
        {
            centerX++;
        }
    }
    recursiveFill(this->_voxels[centerX][centerY][centerZ], centerX, centerY, centerZ);
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

void Voxelization::recursiveFill(VoxelModel* v, const int x, const int y, int z)
{
    if(v->getStatus() == VoxelStatus::OCCUPIED || v->getStatus() == VoxelStatus::INNER)
    {
        return;
    }
    v->setStatus(VoxelStatus::INNER);

    if(x + 1 < _numX)
    {
        recursiveFill(this->_voxels[x + 1][y][z], x + 1, y, z);
    }
    if(x - 1 > 0)
    {
        recursiveFill(this->_voxels[x - 1][y][z], x - 1, y, z);
    }
    if(y + 1 < _numY)
    {
        recursiveFill(this->_voxels[x][y + 1][z], x, y + 1, z);
    }
    if(y - 1 > 0)
    {
        recursiveFill(this->_voxels[x][y - 1][z], x, y - 1, z);
    }
    if(z + 1 < _numZ)
    {
        recursiveFill(this->_voxels[x][y][z + 1], x, y, z + 1);
    }
    if(z - 1 > 0)
    {
        recursiveFill(this->_voxels[x][y][z - 1], x, y, z - 1);
    }
}

AlgGeom::DrawVoxelization* Voxelization::getRenderingObject(const bool useColors, const bool showOuterVoxeles)
{
    VoxelStatus status;
    if(showOuterVoxeles)
        status = VoxelStatus::INNER;
    else
        status = VoxelStatus::OCCUPIED;

    std::vector<glm::vec3> positions;
    const Vect3d           displace(_size[0] / 2, _size[1] / 2, _size[2] / 2);
    const unsigned         numVoxels = _numX * _numY * _numZ;
    glm::vec3*             colors    = new glm::vec3[numVoxels];
    constexpr glm::vec3    color[]   = {glm::vec3(1.0f), glm::vec3(.5f), glm::vec3(1.0f)};

    for(int x = 0; x < _numX; x++)
    {
        for(int y = 0; y < _numY; y++)
        {
            for(int z = 0; z < _numZ; z++)
            {
                if(this->getVoxels()[x][y][z]->getStatus() == status)
                {
                    Vect3d center = this->getVoxels()[x][y][z]->getMin().add(displace);
                    positions.emplace_back(center.getX(), center.getY(), center.getZ());
                    if(useColors)
                    {
                        const unsigned linearIndex = z + y * _numZ + x * _numZ * _numY;
                        colors[linearIndex]        = color[this->getVoxels()[x][y][z]->getStatus()];
                    }
                }
            }
        }
    }

    AlgGeom::DrawVoxelization* voxelization = new AlgGeom::DrawVoxelization(positions.data(), static_cast<int>(positions.size()), _size, useColors ? colors : nullptr);

    delete[] colors;
    return voxelization;
}

bool Voxelization::rayTraversal(Ray3d& r, std::vector<VoxelModel*>& v)
{
    double tMin;
    double tMax;

    if(!rayBoxIntersection(r, tMin, tMax, 0, 1))
        return false;

    tMin = BasicGeometry::max2(tMin, 0);
    tMax = BasicGeometry::max2(tMax, 1);

    const Vect3d rayStart = r.getDirection().scalarMul(tMin).add(r.getOrigin());
    const Vect3d rayEnd   = r.getDirection().scalarMul(tMax).add(r.getOrigin());

    size_t currentXIndex = static_cast<size_t>(BasicGeometry::max2(1, std::ceil((rayStart.getX() - this->getXMin()) / this->_size.x)));
    int    stepX;
    double tDeltaX;
    double tMaxX;
    if(r.getDirection().getX() > 0.0)
    {
        stepX   = 1;
        tDeltaX = this->_size[0] / r.getDirection().getX();
        tMaxX   = tMin + (this->getXMin() + static_cast<float>(currentXIndex) * this->_size.x - rayStart.getX()) / r.getDirection().getX();
    }
    else if(r.getDirection().getX() < 0.0)
    {
        stepX                         = -1;
        tDeltaX                       = this->_size[0] / -r.getDirection().getX();
        const size_t previous_X_index = currentXIndex - 1;
        tMaxX                         = tMin + (this->getXMin() + static_cast<float>(previous_X_index) * this->_size.x - rayStart.getX()) / r.getDirection().getX();
    }
    else
    {
        stepX   = 0;
        tDeltaX = tMax;
        tMaxX   = tMax;
    }

    size_t currentYIndex = static_cast<size_t>(BasicGeometry::max2(1, std::ceil((rayStart.getY() - this->getYMin()) / this->_size.y)));
    int    stepY;
    double tDeltaY;
    double tMaxY;
    if(r.getDirection().getY() > 0.0)
    {
        stepY   = 1;
        tDeltaY = this->_size[1] / r.getDirection().getY();
        tMaxY   = tMin + (this->getYMin() + static_cast<float>(currentYIndex) * this->_size.y - rayStart.getY()) / r.getDirection().getY();
    }
    else if(r.getDirection().getY() < 0.0)
    {
        stepY                         = -1;
        tDeltaY                       = this->_size[1] / -r.getDirection().getY();
        const size_t previous_Y_index = currentYIndex - 1;
        tMaxY                         = tMin + (this->getYMin() + static_cast<float>(previous_Y_index) * this->_size.y - rayStart.getY()) / r.getDirection().getY();
    }
    else
    {
        stepY   = 0;
        tDeltaY = tMax;
        tMaxY   = tMax;
    }

    size_t currentZIndex = static_cast<size_t>(BasicGeometry::max2(1, std::ceil((rayStart.getZ() - this->getZMin()) / this->_size.z)));
    int    stepZ;
    double tDeltaZ;
    double tMaxZ;
    if(r.getDirection().getZ() > 0.0)
    {
        stepZ   = 1;
        tDeltaZ = this->_size[2] / r.getDirection().getZ();
        tMaxZ   = tMin + (this->getZMin() + static_cast<float>(currentZIndex) * this->_size.z - rayStart.getZ()) / r.getDirection().getZ();
    }
    else if(r.getDirection().getZ() < 0.0)
    {
        stepZ                         = -1;
        tDeltaZ                       = this->_size[2] / -r.getDirection().getZ();
        const size_t previous_Z_index = currentZIndex - 1;
        tMaxZ                         = tMin + (this->getZMin() + static_cast<float>(previous_Z_index) * this->_size.z - rayStart.getZ()) / r.getDirection().getZ();
    }
    else
    {
        stepZ   = 0;
        tDeltaZ = tMax;
        tMaxZ   = tMax;
    }

    while(glm::all(glm::greaterThanEqual(glm::vec3(currentXIndex, currentYIndex, currentZIndex), glm::vec3(1, 1, 1))) && glm::all(glm::lessThanEqual(glm::vec3(currentXIndex, currentYIndex, currentZIndex), glm::vec3(_numX, _numY, _numZ))))
    {
        v.push_back(this->_voxels[currentXIndex - 1][currentYIndex - 1][currentZIndex - 1]);
        if(tMaxX < tMaxY && tMaxX < tMaxZ)
        {
            currentXIndex += stepX;
            tMaxX += tDeltaX;
        }
        else if(tMaxY < tMaxZ)
        {
            currentYIndex += stepY;
            tMaxY += tDeltaY;
        }
        else
        {
            currentZIndex += stepZ;
            tMaxZ += tDeltaZ;
        }
    }

    if(v.empty())
        return false;

    return true;
}

bool Voxelization::rayBoxIntersection(Ray3d& r, double& tMin, double& tMax, double t0, double t1) const
{
    double       tYMin, tYMax, tZMin, tZMax;
    const double invertedX = 1 / r.getDirection().getX();
    if(invertedX >= 0)
    {
        tMin = (this->getXMin() - r.getOrigin().getX()) * invertedX;
        tMax = (this->getXMax() - r.getOrigin().getX()) * invertedX;
    }
    else
    {
        tMin = (this->getXMax() - r.getOrigin().getX()) * invertedX;
        tMax = (this->getXMin() - r.getOrigin().getX()) * invertedX;
    }

    const double invertedY = 1 / r.getDirection().getY();
    if(invertedY >= 0)
    {
        tYMin = (this->getYMin() - r.getOrigin().getY()) * invertedY;
        tYMax = (this->getYMax() - r.getOrigin().getY()) * invertedY;
    }
    else
    {
        tYMin = (this->getYMax() - r.getOrigin().getY()) * invertedY;
        tYMax = (this->getYMin() - r.getOrigin().getY()) * invertedY;
    }

    if(tMin > tYMax || tYMin > tMax)
        return false;

    if(tYMin > tMin)
        tMin = tYMin;

    if(tYMax < tMax)
        tMax = tYMax;

    const double invertedZ = 1 / r.getDirection().getZ();
    if(invertedZ >= 0)
    {
        tZMin = (this->getZMin() - r.getOrigin().getZ()) * invertedZ;
        tZMax = (this->getZMax() - r.getOrigin().getZ()) * invertedZ;
    }
    else
    {
        tZMin = (this->getZMax() - r.getOrigin().getZ()) * invertedZ;
        tZMax = (this->getZMin() - r.getOrigin().getZ()) * invertedZ;
    }

    if(tMin > tZMax || tZMin > tMax)
        return false;

    if(tZMin > tMin)
        tMin = tZMin;

    if(tZMax < tMax)
        tMax = tZMax;

    return tMin < t1 && tMax > t0;
}
