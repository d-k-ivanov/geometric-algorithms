#include "PointCloud3d.h"
#include "BasicGeometry.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace GDSA::Geometry
{
PointCloud3d::PointCloud3d()
    : _maxPoint(-INFINITY, -INFINITY, -INFINITY)
    , _minPoint(INFINITY, INFINITY, INFINITY)
{
}

PointCloud3d::PointCloud3d(const std::vector<Vect3d>& points)
    : _points(points)
    , _maxPoint(-INFINITY, -INFINITY, -INFINITY)
    , _minPoint(INFINITY, INFINITY, INFINITY)
{
}

PointCloud3d::PointCloud3d(const std::string& filename)
    : _maxPoint(-INFINITY, -INFINITY, -INFINITY)
    , _minPoint(INFINITY, INFINITY, INFINITY)
{
    auto splitByComma = [this](const std::string& string) -> std::vector<std::string>
    {
        std::stringstream        ss(string);
        std::vector<std::string> result;

        while(ss.good())
        {
            std::string substr;
            getline(ss, substr, ',');
            result.push_back(substr);
        }

        return result;
    };

    std::string   currentLine;
    std::ifstream inputStream;
    inputStream.open(filename.c_str());

    while(std::getline(inputStream, currentLine))
    {
        std::vector<std::string> coord = splitByComma(currentLine);

        if(coord.size() == 3)
        {
            try
            {
                Vect3d point(std::stof(coord[0], nullptr), std::stof(coord[1], nullptr), (std::stof(coord[2], nullptr)));
                this->addPoint(point);
            }
            catch(const std::exception& e)
            {
                inputStream.close();
                throw e;
            }
        }
    }
    inputStream.close();
}

PointCloud3d::PointCloud3d(int size, const double maxX, const double maxY, const double maxZ)
    : _maxPoint(-INFINITY, -INFINITY, -INFINITY)
    , _minPoint(INFINITY, INFINITY, INFINITY)
{
    _points = std::vector<Vect3d>();

    while(size > 0)
    {
        const double x = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (maxX * 2.0f))) - maxX;
        const double y = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (maxY * 2.0f))) - maxY;
        const double z = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (maxZ * 2.0f))) - maxZ;
        Vect3d       val(x, y, z);
        this->addPoint(val);

        --size;
    }
}

PointCloud3d::PointCloud3d(int size, const double radius)
    : _maxPoint(-INFINITY, -INFINITY, -INFINITY)
    , _minPoint(INFINITY, INFINITY, INFINITY)
{
    _points = std::vector<Vect3d>();

    while(size > 0)
    {
        const double theta = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX)) * 2.0 * glm::pi<double>();
        const double phi   = std::acos(1.0 - 2.0 * static_cast<double>(rand()) / (static_cast<double>(RAND_MAX)));
        const double x     = std::sin(phi) * std::cos(theta);
        const double y     = std::sin(phi) * std::sin(theta);
        const double z     = std::cos(phi);
        const double r     = radius * std::sqrt(static_cast<double>(rand()) / (static_cast<double>(RAND_MAX)));

        Vect3d point(r * x, r * y, r * z);
        this->addPoint(point);

        --size;
    }
}

PointCloud3d::PointCloud3d(const PointCloud3d& pointCloud)
    : _points(pointCloud._points)
    , _maxPoint(pointCloud._maxPoint)
    , _minPoint(pointCloud._minPoint)
{
}

PointCloud3d& PointCloud3d::operator=(const PointCloud3d& pointCloud)
{
    if(this != &pointCloud)
    {
        _points   = pointCloud._points;
        _maxPoint = pointCloud._maxPoint;
        _minPoint = pointCloud._minPoint;
    }

    return *this;
}

void PointCloud3d::addPoint(const Vect3d& p)
{
    _points.push_back(p);
    this->updateMaxMin(_points.size() - 1);
}

AABB PointCloud3d::getAABB() const
{
    return {_minPoint, _maxPoint};
}

Vect3d PointCloud3d::getPoint(const size_t pos)
{
    if((pos < _points.size()))
    {
        return _points[pos];
    }
    return {};
}

std::vector<Vect3d> PointCloud3d::getPoints()
{
    return _points;
}

void PointCloud3d::save(const std::string& filename) const
{
    std::ofstream file(filename);

    for(int i = 0; i < _points.size(); ++i)
    {
        file << _points[i].getX() << ", " << _points[i].getY() << ", " << _points[i].getZ() << '\n';
    }

    file.close();
}

void PointCloud3d::updateMaxMin(size_t index)
{
    const Vect3d point = _points[index];

    if(point.getX() < _minPoint.getX())
    {
        _minPoint.setX(point.getX());
        _minPointIndex.setX(static_cast<double>(index));
    }
    if(point.getY() < _minPoint.getY())
    {
        _minPoint.setY(point.getY());
        _minPointIndex.setY(static_cast<double>(index));
    }
    if(point.getZ() < _minPoint.getZ())
    {
        _minPoint.setZ(point.getZ());
        _minPointIndex.setZ(static_cast<double>(index));
    }

    if(point.getX() > _maxPoint.getX())
    {
        _maxPoint.setX(point.getX());
        _maxPointIndex.setX(static_cast<double>(index));
    }
    if(point.getY() > _maxPoint.getY())
    {
        _maxPoint.setY(point.getY());
        _maxPointIndex.setY(static_cast<double>(index));
    }
    if(point.getZ() > _maxPoint.getZ())
    {
        _maxPoint.setZ(point.getZ());
        _maxPointIndex.setZ(static_cast<double>(index));
    }
}

void PointCloud3d::getMostDistanced(int& a, int& b) const
{
    a = 0;
    b = 0;

    double maxDistance = -INFINITY;
    for(int i = 0; i < _points.size(); ++i)
    {
        for(int j = i + 1; j < _points.size(); ++j)
        {
            const double distance = _points[i].distance(_points[j]);
            if(distance > maxDistance)
            {
                maxDistance = distance;
                a           = i;
                b           = j;
            }
        }
    }
}
}    // namespace GDSA::Geometry
