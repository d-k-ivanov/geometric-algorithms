#include "VoxelModel.h"

#include "AABB.h"
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
    for(size_t i = 0; i < _points.size(); i++)
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
    const glm::vec3 boxHalfSize(_size[0] / 2, _size[1] / 2, _size[2] / 2);

    if(triangleOverlap(this->_center, boxHalfSize, vertices))
    {
        return true;
    }
    return false;
}

bool VoxelModel::planeBoxOverlap(const Vect3d& normal, const Vect3d& vertex, glm::vec3 maxPoint) const
{
    Vect3d vmin, vmax;

    if(normal.getX() > 0.0)
    {
        vmin.setX(-maxPoint[0] - vertex.getX());
        vmax.setX(maxPoint[0] - vertex.getX());
    }
    else
    {
        vmin.setX(maxPoint[0] - vertex.getX());
        vmax.setX(-maxPoint[0] - vertex.getX());
    }

    if(normal.getY() > 0.0)
    {
        vmin.setY(-maxPoint[1] - vertex.getY());
        vmax.setY(maxPoint[1] - vertex.getY());
    }
    else
    {
        vmin.setY(maxPoint[1] - vertex.getY());
        vmax.setY(-maxPoint[1] - vertex.getY());
    }

    if(normal.getZ() > 0.0)
    {
        vmin.setZ(-maxPoint[2] - vertex.getZ());
        vmax.setZ(maxPoint[2] - vertex.getZ());
    }
    else
    {
        vmin.setZ(maxPoint[2] - vertex.getZ());
        vmax.setZ(-maxPoint[2] - vertex.getZ());
    }

    if(normal.dot(vmin) > 0.0)
        return false;

    if(normal.dot(vmax) > 0.0 || BasicGeometry ::equal(0.0, normal.dot(vmax)))
        return true;

    return false;
}

bool VoxelModel::triangleOverlap(const Vect3d& center, glm::vec3 size, std::vector<Vect3d> vertices) const
{
    Vect3d v0, v1, v2;
    double min, max, p0, p1, p2, rad, fex, fey, fez;

    Vect3d normal, edge0, edge1, edge2;

    v0 = vertices[0].sub(center);
    v1 = vertices[1].sub(center);
    v2 = vertices[2].sub(center);

    edge0 = v1.sub(v0);
    edge1 = v2.sub(v1);
    edge2 = v0.sub(v2);

    fex = glm::abs(edge0.getX());
    fey = glm::abs(edge0.getY());
    fez = glm::abs(edge0.getZ());

    if(!xAxisTest(edge0.getZ(), edge0.getY(), fez, fey, p0, p2, v0, v2, size, min, max, rad))
        return false;
    if(!yAxisTest(edge0.getZ(), edge0.getX(), fez, fex, p0, p2, v0, v2, size, min, max, rad))
        return false;
    if(!zAxisTest(edge0.getY(), edge0.getX(), fey, fez, p2, p1, v1, v2, size, min, max, rad))
        return false;

    fex = glm::abs(edge1.getX());
    fey = glm::abs(edge1.getY());
    fez = glm::abs(edge1.getZ());

    if(!xAxisTest(edge1.getZ(), edge1.getY(), fez, fey, p0, p2, v0, v2, size, min, max, rad))
        return false;
    if(!yAxisTest(edge1.getZ(), edge1.getX(), fez, fex, p0, p2, v0, v2, size, min, max, rad))
        return false;
    if(!zAxisTest(edge1.getY(), edge1.getX(), fey, fez, p0, p1, v0, v1, size, min, max, rad))
        return false;

    fex = glm::abs(edge2.getX());
    fey = glm::abs(edge2.getY());
    fez = glm::abs(edge2.getZ());

    if(!xAxisTest(edge2.getZ(), edge2.getY(), fez, fey, p0, p1, v0, v1, size, min, max, rad))
        return false;
    if(!yAxisTest(edge2.getZ(), edge2.getX(), fez, fex, p0, p1, v0, v1, size, min, max, rad))
        return false;
    if(!zAxisTest(edge2.getY(), edge2.getX(), fey, fez, p2, p1, v1, v2, size, min, max, rad))
        return false;

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

    normal = edge0.xProduct(edge1);

    if(!planeBoxOverlap(normal, v0, size))
        return false;

    return true;
}

bool VoxelModel::xAxisTest(const double& a, const double& b, const double& fa, const double& fb, double& p0, double& px, const Vect3d& v0, const Vect3d& vx, const glm::vec3& boxRadius, double& min, double& max, double& rad) const
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

    rad = fa * boxRadius.y + fb * boxRadius.z;

    if(min > rad || max < -rad)
        return false;

    return true;
}

bool VoxelModel::yAxisTest(const double& a, const double& b, const double& fa, const double& fb, double& p0, double& px, const Vect3d& v0, const Vect3d& vx, const glm::vec3& boxRadius, double& min, double& max, double& rad) const
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

    rad = fa * boxRadius.x + fb * boxRadius.z;

    if(min > rad || max < -rad)
        return false;

    return true;
}

bool VoxelModel::zAxisTest(const double& a, const double& b, const double& fa, const double& fb, double& p0, double& px, const Vect3d& v0, const Vect3d& vx, const glm::vec3& boxRadius, double& min, double& max, double& rad) const
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

    rad = fa * boxRadius.x + fb * boxRadius.y;

    if(min > rad || max < -rad)
        return false;

    return true;
}
