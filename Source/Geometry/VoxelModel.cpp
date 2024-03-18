#include "VoxelModel.h"

#include "BasicGeometry.h"

#include "Utils/ChronoUtilities.h"

VoxelModel::VoxelModel()
{
    _status = VoxelStatus::NP;
    _points = std::vector<Vect3d>();
    _size   = glm::vec3(0, 0, 0);
}

VoxelModel::VoxelModel(const Vect3d& minPoint, glm::vec3 size)
{
    _status     = VoxelStatus::NP;
    _center     = {minPoint.getX() + size[0] / 2, minPoint.getY() + size[1] / 2, minPoint.getZ() + size[2] / 2};
    this->_min  = minPoint;
    this->_max  = {_min.getX() + size[0], _min.getY() + size[1], _min.getZ() + size[2]};
    this->_size = size;
}

VoxelModel::VoxelModel(const VoxelModel& voxel)
{
    _status = voxel._status;
    _points = voxel._points;
    _size   = voxel._size;
}

void VoxelModel::add(const Vect3d& point)
{
    _points.insert(_points.end(), point);
}

bool VoxelModel::find(const Vect3d& point)
{
    for(int i = 0; i < _points.size(); i++)
    {
        if(_points[i] == point)
        {
            return true;
        }
    }
    return false;
}

bool VoxelModel::remove(const Vect3d& point)
{
    const std::vector<Vect3d>::iterator it = _points.begin();
    while(it != _points.end())
    {
        if(*it == point)
        {
            _points.erase(it);
            return true;
        }
    }
    return false;
}

bool VoxelModel::bruteForce(Triangle3d triangle)
{
    std::vector<Vect3d> vertices;
    vertices.push_back(triangle.getA());
    vertices.push_back(triangle.getB());
    vertices.push_back(triangle.getC());
    const glm::vec3 halfsize(_size[0] / 2, _size[1] / 2, _size[2] / 2);

    if(triBoxOverlap(this->_center, halfsize, vertices))
    {
        return true;
    }
    return false;
}

bool VoxelModel::planeBoxOverlap(const Vect3d& normal, const Vect3d& vert, glm::vec3 maxPoint) const
{
    Vect3d vmin, vmax, v;
    v = vert;

    if(normal.getX() > 0.0f)
    {
        vmin.setX(-maxPoint[0] - v.getX());
        vmax.setX(maxPoint[0] - v.getX());
    }
    else
    {
        vmin.setX(maxPoint[0] - v.getX());
        vmax.setX(-maxPoint[0] - v.getX());
    }

    if(normal.getY() > 0.0f)
    {
        vmin.setY(-maxPoint[1] - v.getY());
        vmax.setY(maxPoint[1] - v.getY());
    }
    else
    {
        vmin.setY(maxPoint[1] - v.getY());
        vmax.setY(-maxPoint[1] - v.getY());
    }

    if(normal.getZ() > 0.0f)
    {
        vmin.setZ(-maxPoint[2] - v.getZ());
        vmax.setZ(maxPoint[2] - v.getZ());
    }
    else
    {
        vmin.setZ(maxPoint[2] - v.getZ());
        vmax.setZ(-maxPoint[2] - v.getZ());
    }

    if(normal.dot(vmin) > 0.0f)
        return false;

    if(normal.dot(vmax) >= 0.0f)
        return true;

    return false;
}

bool VoxelModel::triBoxOverlap(const Vect3d& center, glm::vec3 size, std::vector<Vect3d> vertices)
{
    Vect3d v0, v1, v2;
    float  min, max, p0, p1, p2, rad, fex, fey, fez;

    Vect3d normal, e0, e1, e2;

    v0 = vertices[0].sub(center);
    v1 = vertices[1].sub(center);
    v2 = vertices[2].sub(center);

    e0 = v1.sub(v0);
    e1 = v2.sub(v1);
    e2 = v0.sub(v2);

    fex = glm::abs(e0.getX());
    fey = glm::abs(e0.getY());
    fez = glm::abs(e0.getZ());

    double aux  = e0.getZ();
    double aux2 = e0.getY();
    double aux3 = e0.getX();

    axisTestX(aux, aux2, fez, fey, p0, p2, v0, v2, size, min, max, rad);
    axisTestY(aux, aux3, fez, fex, p0, p2, v0, v2, size, min, max, rad);
    axisTestZ(aux2, aux3, fey, fez, p2, p1, v1, v2, size, min, max, rad);

    fex = glm::abs(e1.getX());
    fey = glm::abs(e1.getY());
    fez = glm::abs(e1.getZ());

    aux  = e1.getZ();
    aux2 = e1.getY();
    aux3 = e1.getX();

    axisTestX(aux, aux2, fez, fey, p0, p2, v0, v2, size, min, max, rad);
    axisTestY(aux, aux3, fez, fex, p0, p2, v0, v2, size, min, max, rad);
    axisTestZ(aux2, aux3, fey, fez, p0, p1, v0, v1, size, min, max, rad);

    fex = glm::abs(e2.getX());
    fey = glm::abs(e2.getY());
    fez = glm::abs(e2.getZ());

    aux  = e2.getZ();
    aux2 = e2.getY();
    aux3 = e2.getX();

    axisTestX(aux, aux2, fez, fey, p0, p1, v0, v1, size, min, max, rad);
    axisTestY(aux, aux3, fez, fex, p0, p1, v0, v1, size, min, max, rad);
    axisTestZ(aux2, aux3, fey, fez, p2, p1, v1, v2, size, min, max, rad);

    min = BasicGeometry::min3(v0.getX(), v1.getX(), v2.getX());
    max = BasicGeometry::max3(v0.getX(), v1.getX(), v2.getX());

    if(min > size[0] || max < -size[0])
        return false;

    min = BasicGeometry::min3(v0.getY(), v1.getY(), v2.getY());
    max = BasicGeometry::max3(v0.getY(), v1.getY(), v2.getY());

    if(min > size[1] || max < -size[1])
        return false;

    min = BasicGeometry::min3(v0.getZ(), v1.getZ(), v2.getZ());
    max = BasicGeometry::max3(v0.getZ(), v1.getZ(), v2.getZ());

    if(min > size[2] || max < -size[2])
        return false;

    normal = e0.xProduct(e1);

    if(!planeBoxOverlap(normal, v0, size))
        return false;

    return true;
}

bool VoxelModel::axisTestX(const double& a, const double& b, const float& fa, const float& fb, float& p0, float& px, const Vect3d& v0, const Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad) const
{
    p0 = a * v0.getY() - b * v0.getZ();
    px = a * vx.getY() - b * vx.getZ();

    if(p0 < px)
    {
        min = p0;
        max = px;
    }
    else
    {
        min = px;
        max = p0;
    }

    rad = fa * boxHalfSize[1] + fb * boxHalfSize[2];

    if(min > rad || max < -rad)
        return false;

    return true;
}

bool VoxelModel::axisTestY(const double& a, const double& b, const float& fa, const float& fb, float& p0, float& px, const Vect3d& v0, const Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad) const
{
    p0 = -a * v0.getX() + b * v0.getZ();
    px = -a * vx.getX() + b * vx.getZ();

    if(p0 < px)
    {
        min = p0;
        max = px;
    }
    else
    {
        min = px;
        max = p0;
    }

    rad = fa * boxHalfSize[0] + fb * boxHalfSize[2];

    if(min > rad || max < -rad)
        return false;

    return true;
}

bool VoxelModel::axisTestZ(const double& a, const double& b, const float& fa, const float& fb, float& p0, float& px, const Vect3d& v0, const Vect3d& vx, glm::vec3& boxHalfSize, float& min, float& max, float& rad) const
{
    p0 = a * v0.getX() - b * v0.getY();
    px = a * vx.getX() - b * vx.getY();

    if(p0 < px)
    {
        min = p0;
        max = px;
    }
    else
    {
        min = px;
        max = p0;
    }

    rad = fa * boxHalfSize[0] + fb * boxHalfSize[1];

    if(min > rad || max < -rad)
        return false;

    return true;
}
