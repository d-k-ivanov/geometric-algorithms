#include "Voxelization.h"

#include "Utils/RandomUtilities.h"

// Public methods

Voxelization::Voxelization(TriangleModel* model, const glm::vec3& voxelSize)
    : _model(model)
    , _voxelSize(voxelSize)
{
    AABB      aabb     = model->getAABB();
    glm::vec3 aabbSize = aabb.getSize().toGlmVec3();
    _splits            = glm::ceil(aabbSize / voxelSize);
    _min               = aabb.getCenter().toGlmVec3() - glm::vec3(_splits) / glm::vec3(2.0f) * _voxelSize;
    _max               = aabb.getCenter().toGlmVec3() + glm::vec3(_splits) / glm::vec3(2.0f) * _voxelSize;
    _aabb              = AABB(Vect3d(_min.x, _min.y, _min.z), Vect3d(_max.x, _max.y, _max.z));

    _voxel = new Voxel**[_splits.x];
    for(unsigned x = 0; x < _splits.x; ++x)
    {
        _voxel[x] = new Voxel*[_splits.y];
        for(unsigned y = 0; y < _splits.y; ++y)
        {
            _voxel[x][y] = new Voxel[_splits.z];
            for(unsigned z = 0; z < _splits.z; ++z)
            {
                _min                  = _aabb.getMin().toGlmVec3() + glm::vec3(x, y, z) * _voxelSize;
                _max                  = _aabb.getMin().toGlmVec3() + glm::vec3(x + 1, y + 1, z + 1) * _voxelSize;
                _voxel[x][y][z]._aabb = AABB(Vect3d(_min.x, _min.y, _min.z), Vect3d(_max.x, _max.y, _max.z));
            }
        }
    }
}

Voxelization::~Voxelization()
{
    for(unsigned x = 0; x < _splits.x; ++x)
    {
        for(unsigned y = 0; y < _splits.y; ++y)
        {
            delete[] _voxel[x][y];
        }
        delete _voxel[x];
    }
    delete[] _voxel;
}

void Voxelization::bruteForce() const
{
    unsigned triangleIdx = 0;
    auto     triangles   = _model->getFaces();

    for(Triangle3d& triangle : triangles)
    {
        for(unsigned x = 0; x < _splits.x; ++x)
        {
            for(unsigned y = 0; y < _splits.y; ++y)
            {
                for(unsigned z = 0; z < _splits.z; ++z)
                {
                    if(Intersections3d::intersectTA(triangle, _voxel[x][y][z]._aabb))
                    {
                        _voxel[x][y][z]._triangles.insert(triangleIdx);
                        _voxel[x][y][z]._status = Voxel::OCCUPIED;
                    }
                }
            }
        }

        ++triangleIdx;
    }
}

void Voxelization::flood()
{
    glm::ivec3                                                          centre = _splits / glm::uvec3(2);
    std::unordered_set<std::pair<glm::ivec3, Voxelization*>, VoxelHash> voxels {std::make_pair(centre, this)};

    while(!voxels.empty())
    {
        std::unordered_set<std::pair<glm::ivec3, Voxelization*>, VoxelHash> newVoxels;

        while(!voxels.empty())
        {
            auto voxel = *voxels.begin();
            voxels.erase(voxels.begin());
            this->flood(voxel.first.x, voxel.first.y, voxel.first.z, newVoxels);
        }

        for(auto& voxelPair : newVoxels)
        {
            voxels.insert(voxelPair);
        }
    }
}

AlgGeom::DrawVoxelization* Voxelization::getRenderingObject(bool useColors) const
{
    AlgGeom::DrawVoxelization* voxelization = nullptr;

    if(useColors)
    {
        const unsigned      numVoxels = _splits.x * _splits.y * _splits.z;
        glm::vec3*          positions = new glm::vec3[numVoxels];
        glm::vec3*          colors    = new glm::vec3[numVoxels];
        constexpr glm::vec3 color[]   = {glm::vec3(.0f), glm::vec3(.5f), glm::vec3(1.0f)};

        for(unsigned x = 0; x < _splits.x; ++x)
        {
            for(unsigned y = 0; y < _splits.y; ++y)
            {
                for(unsigned z = 0; z < _splits.z; ++z)
                {
                    const unsigned linearIndex = z + y * _splits.z + x * _splits.z * _splits.y;
                    positions[linearIndex]     = _voxel[x][y][z]._aabb.getCenter().toGlmVec3();
                    colors[linearIndex]        = color[_voxel[x][y][z]._status];
                }
            }
        }

        voxelization = new AlgGeom::DrawVoxelization(positions, numVoxels, _voxelSize, colors);
        delete[] positions;
        delete[] colors;
    }
    else
    {
        std::vector<glm::vec3> positions;

        for(unsigned x = 0; x < _splits.x; ++x)
        {
            for(unsigned y = 0; y < _splits.y; ++y)
            {
                for(unsigned z = 0; z < _splits.z; ++z)
                {
                    if(_voxel[x][y][z]._status == Voxel::INNER)
                        positions.push_back(_voxel[x][y][z]._aabb.getCenter().toGlmVec3());
                }
            }
        }

        voxelization = new AlgGeom::DrawVoxelization(positions.data(), positions.size(), _voxelSize, nullptr);
    }

    return voxelization;
}

void Voxelization::printData()
{
    unsigned numOccupiedVoxels = 0;
    unsigned numInnerVoxels    = 0;
    unsigned numOuterVoxels    = 0;

    for(unsigned x = 0; x < _splits.x; ++x)
    {
        for(unsigned y = 0; y < _splits.y; ++y)
        {
            for(unsigned z = 0; z < _splits.z; ++z)
            {
                numOccupiedVoxels += static_cast<unsigned>(_voxel[x][y][z]._status == Voxel::OCCUPIED);
                numInnerVoxels += static_cast<unsigned>(_voxel[x][y][z]._status == Voxel::INNER);
                numOuterVoxels += static_cast<unsigned>(_voxel[x][y][z]._status == Voxel::OUTER);
            }
        }
    }

    std::cout << "Number of filled voxels: " << numOccupiedVoxels << '\n';
    std::cout << "Number of inner voxels: " << numInnerVoxels << '\n';
    std::cout << "Number of outer voxels: " << numOuterVoxels << '\n';
}

void Voxelization::voxelNeighbourhood()
{
    unsigned triangleIdx = 0;
    auto     triangles   = _model->getFaces();

    for(Triangle3d& triangle : triangles)
    {
        const glm::ivec3 minIndices = this->getIndices(triangle.getAABB().getMin().toGlmVec3());
        const glm::ivec3 maxIndices = this->getIndices(triangle.getAABB().getMax().toGlmVec3());

        for(unsigned x = minIndices.x; x <= maxIndices.x; ++x)
        {
            for(unsigned y = minIndices.y; y <= maxIndices.y; ++y)
            {
                for(unsigned z = minIndices.z; z <= maxIndices.z; ++z)
                {
                    if(Intersections3d::intersectTA(triangle, _voxel[x][y][z]._aabb))
                    {
                        _voxel[x][y][z]._triangles.insert(triangleIdx);
                        _voxel[x][y][z]._status = Voxel::OCCUPIED;
                    }
                }
            }
        }

        ++triangleIdx;
    }
}

void Voxelization::sampleTriangle(unsigned numSamples)
{
    std::vector<glm::vec2> randomFloats;
    for(unsigned idx = 0; idx < numSamples; ++idx)
    {
        glm::vec2 uv(RandomUtilities::getUniformRandom(), RandomUtilities::getUniformRandom());
        if(uv.x + uv.y >= 1.0f)
        {
            uv = 1.0f - uv;
        }
        randomFloats.push_back(uv);
    }

    glm::vec3 point;
    unsigned  triangleIdx = 0;
    auto      triangles   = _model->getFaces();

    for(Triangle3d& triangle : triangles)
    {
        for(unsigned sampleIdx = 0; sampleIdx < numSamples; ++sampleIdx)
        {
            point = triangle.samplePoint(randomFloats[sampleIdx].x, randomFloats[sampleIdx].y).toGlmVec3();
            this->insertPoint(point, triangleIdx);
        }

        ++triangleIdx;
    }
}

void Voxelization::sweep() const
{
    unsigned minIndex = 0, maxIndex = 0;
    auto     triangles = _model->getFaces();
    std::sort(triangles.begin(), triangles.end(), TriangleComparison());

    for(unsigned y = 0; y < _splits.y; ++y)
    {
        const float max_y = _voxel[0][y][0]._aabb.getMax()._y + glm::epsilon<float>();
        const float min_y = _voxel[0][y][0]._aabb.getMin()._y + glm::epsilon<float>();

        while(maxIndex < triangles.size() && triangles[maxIndex].getAABB().getMin()._y < max_y)
            ++maxIndex;

        while(triangles[minIndex].getAABB().getMax()._y < min_y)
            ++minIndex;

        for(unsigned idx = minIndex; idx < maxIndex; ++idx)
        {
            for(unsigned x = 0; x < _splits.x; ++x)
            {
                for(unsigned z = 0; z < _splits.z; ++z)
                {
                    if(Intersections3d::intersectTA(triangles[idx], _voxel[x][y][z]._aabb))
                    {
                        _voxel[x][y][z]._triangles.insert(idx);
                        _voxel[x][y][z]._status = Voxel::OCCUPIED;
                    }
                }
            }
        }
    }
}

void Voxelization::testRayTraversedVoxels(const Ray3d& ray, std::vector<AABB>& traversedVoxels) const
{
    const EisemannRay eisRay(ray);

    for(unsigned x = 0; x < _splits.x; ++x)
    {
        for(unsigned y = 0; y < _splits.y; ++y)
        {
            for(unsigned z = 0; z < _splits.z; ++z)
            {
                if(Intersections3d::intersectAR(_voxel[x][y][z]._aabb, eisRay))
                    traversedVoxels.push_back(_voxel[x][y][z]._aabb);
            }
        }
    }
}

void Voxelization::testRayTraversedVoxelsAccelerated(Ray3d& ray, std::vector<AABB>& traversedVoxels)
{
    glm::uvec3 voxel = this->getIndices(ray.getOrigin().toGlmVec3()), previousVoxel = glm::uvec3(UINT_MAX);
    glm::vec3  rayPosition = ray.getOrigin().toGlmVec3();

    while(glm::all(glm::greaterThanEqual(voxel, glm::uvec3(0))) && glm::all(glm::lessThan(voxel, _splits)))
    {
        if(!glm::all(glm::equal(voxel, previousVoxel)))
        {
            traversedVoxels.push_back(_voxel[voxel.x][voxel.y][voxel.z]._aabb);
            previousVoxel = voxel;
        }

        rayPosition += _voxelSize * glm::normalize(ray.getDirection().toGlmVec3()) / 50.0f;
        voxel = this->getIndices(rayPosition);
    }
}

void Voxelization::testRayTraversedVoxelsAcceleratedNeighbourhood(Ray3d& ray, std::vector<AABB>& traversedVoxels)
{
    EisemannRay eissRay(ray);
    float       distance;
    glm::ivec3  voxel = this->getIndices(ray.getOrigin().toGlmVec3()), previousVoxel = glm::uvec3(UINT_MAX), splits = _splits - glm::uvec3(1), closestVoxel;
    glm::vec3   rayPosition  = ray.getOrigin().toGlmVec3();
    glm::vec3   rayDirection = ray.getDirection().toGlmVec3();
    glm::ivec3  minIndices   = glm::uvec3(static_cast<unsigned>(rayDirection.x < .0f), static_cast<unsigned>(rayDirection.y < .0f), static_cast<unsigned>(rayDirection.z < .0f));
    minIndices *= -1;
    glm::ivec3 maxIndices = glm::uvec3(static_cast<unsigned>(rayDirection.x > .0f), static_cast<unsigned>(rayDirection.y > .0f), static_cast<unsigned>(rayDirection.z > .0f));

    traversedVoxels.push_back(_voxel[voxel.x][voxel.y][voxel.z]._aabb);

    while(true)
    {
        float      minDistance = FLT_MAX;
        glm::ivec3 minVoxel = glm::clamp(voxel + minIndices, glm::ivec3(0), splits), maxVoxel = glm::clamp(voxel + maxIndices, glm::ivec3(0), splits);
        closestVoxel = glm::ivec3(-1);

        for(int x = minVoxel.x; x <= maxVoxel.x; ++x)
        {
            for(int y = minVoxel.y; y <= maxVoxel.y; ++y)
            {
                for(int z = minVoxel.z; z <= maxVoxel.z; ++z)
                {
                    if(!glm::all(glm::equal(voxel, glm::ivec3(x, y, z))) && Intersections3d::intersectAR(_voxel[x][y][z]._aabb, eissRay))
                    {
                        distance = glm::distance2(_voxel[voxel.x][voxel.y][voxel.z]._aabb.getCenter().toGlmVec3(), _voxel[x][y][z]._aabb.getCenter().toGlmVec3());
                        if(distance < minDistance)
                        {
                            minDistance  = distance;
                            closestVoxel = glm::ivec3(x, y, z);
                        }
                    }
                }
            }
        }

        if(closestVoxel.x >= 0)
        {
            traversedVoxels.push_back(_voxel[closestVoxel.x][closestVoxel.y][closestVoxel.z]._aabb);
            voxel = closestVoxel;
        }
        else
            break;
    }
}

// Protected methods

void Voxelization::flood(int x, int y, int z, std::unordered_set<std::pair<glm::ivec3, Voxelization*>, VoxelHash>& exploredVoxels)
{
    if(_voxel[x][y][z]._status == Voxel::OCCUPIED)
        return;

    _voxel[x][y][z]._status = Voxel::INNER;

    for(int x_idx = x - 1; x_idx < x + 2; ++x_idx)
    {
        if(x_idx >= 0 && x_idx < _splits.x)
        {
            for(int y_idx = y - 1; y_idx < y + 2; ++y_idx)
            {
                if(y_idx >= 0 && y_idx < _splits.y)
                {
                    for(int z_idx = z - 1; z_idx < z + 2; ++z_idx)
                    {
                        if(z_idx >= 0 && z_idx < _splits.z && !(x == x_idx && y == y_idx && z == z_idx) && _voxel[x_idx][y_idx][z_idx]._status != Voxel::INNER)
                        {
                            exploredVoxels.insert(std::make_pair(glm::ivec3(x_idx, y_idx, z_idx), this));
                        }
                    }
                }
            }
        }
    }
}

glm::ivec3 Voxelization::getIndices(const glm::vec3& point)
{
    return {(point - _aabb.getMin().toGlmVec3()) / _voxelSize};
}

void Voxelization::insertPoint(const glm::vec3& point, const unsigned triangleIdx)
{
    const glm::ivec3 indices = this->getIndices(point);
    _voxel[indices.x][indices.y][indices.z]._triangles.insert(triangleIdx);
    _voxel[indices.x][indices.y][indices.z]._status = Voxel::OCCUPIED;
}
