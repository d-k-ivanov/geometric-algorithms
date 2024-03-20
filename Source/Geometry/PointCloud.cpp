#include "PointCloud.h"

#include "Utils/RandomUtilities.h"

#include <fstream>
#include <sstream>

PointCloud::PointCloud()
{
}

PointCloud::PointCloud(int size, double max_x, double max_y)
{
    for(int i = 0; i < size; i++)
    {
        _points.push_back(Point(GDSA::Utils::Random::getUniformRandom(-max_x, max_x), GDSA::Utils::Random::getUniformRandom(-max_y, max_y)));
    }
}

PointCloud::PointCloud(const std::vector<Point>& points)
{
    this->_points = points;
}

PointCloud::PointCloud(const std::string& filename)
{
    std::ifstream file;

    file.open(filename, std::fstream::in);
    std::vector<Point> points;
    int                numPoints;

    // Text file version
    std::string line;
    if(std::getline(file, line))
    {
        numPoints = std::stoi(line);
    }
    else
    {
        // Handle the case where the file is empty or cannot be read
        // For example, you can throw an exception or set a default value for numPoints
        numPoints = 0;
    }

    for(int i = 0; i < numPoints; i++)
    {
        if(std::getline(file, line))
        {
            Point              p;
            std::istringstream iss(line);
            iss >> p;
            points.push_back(p);
        }
        else
        {
            // Handle the case where the file is empty or cannot be read
            // For example, you can throw an exception or set a default value for p
            points.emplace_back();
        }
    }

    // Binary file version
    // file.read(reinterpret_cast<char*>(&numPoints), sizeof(int));
    // std::vector<Point> points;
    // for(int i = 0; i < numPoints; i++)
    // {
    //     Point p;
    //     file.read(reinterpret_cast<char*>(&p), sizeof(Point));
    //     points.push_back(p);
    // }

    file.close();

    this->_points = points;
}

PointCloud::~PointCloud()
{
}

void PointCloud::addPoint(const Point& p)
{
    _points.push_back(p);
}

Point PointCloud::centralPoint()
{
    std::vector<double> distances;
    distances.reserve(_points.size());
    for(size_t i = 0; i < _points.size(); i++)
    {
        distances.push_back(0);
    }

    for(size_t i = 0; i < this->size() - 1; i++)
    {
        for(size_t j = i + 1; j < this->size(); j++)
        {
            distances[i] += _points[i].distance(_points[j]);
            distances[j] += _points[i].distance(_points[j]);
        }
    }

    double minDistance = std::numeric_limits<double>::max();

    size_t index = 0;
    for(size_t i = 1; i < this->size(); i++)
    {
        if(distances[i] < minDistance)
        {
            minDistance = distances[i];
            index       = i;
        }
    }

    return this->getPoint(index);
}

void PointCloud::deletePoint(int index)
{
    if(index < static_cast<int>(_points.size()))
    {
        _points.erase(_points.begin() + index);
    }
}

void PointCloud::getEdges(Point& minPoint_x, Point& minPoint_y, Point& maxPoint_x, Point& maxPoint_y)
{
}

Point PointCloud::getPoint(const size_t position)
{
    if(position < _points.size())
    {
        return _points[position];
    }

    return {};
}

PointCloud& PointCloud::operator=(const PointCloud& pointCloud)
{
    if(this != &pointCloud)
    {
        this->_points = pointCloud._points;
    }

    return *this;
}

void PointCloud::save(const std::string& filename)
{
    std::ofstream file;

    file.open(filename, std::ios::out);
    const int numPoints = static_cast<int>(this->size());

    // Text file version
    file << numPoints << '\n';
    for(int i = 0; i < numPoints; i++)
    {
        file << _points[i] << '\n';
    }

    // Binary file version
    // file.write(reinterpret_cast<char*>(&numPoints), sizeof(int));
    // file.write(reinterpret_cast<char*>(_points.data()), static_cast<int>(numPoints * sizeof(Point)));
    file.close();
}