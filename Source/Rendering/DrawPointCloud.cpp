#include "DrawPointCloud.h"

GDSA::Render::DrawPointCloud::DrawPointCloud(Geometry::PointCloud& pointCloud)
    : Model3D()
{
    size_t     numPoints = pointCloud.size();
    Component* component = new Component;

    for(unsigned vertexIdx = 0; vertexIdx < numPoints; vertexIdx++)
    {
        Geometry::Point point = pointCloud.getPoint(vertexIdx);
        component->_vertices.push_back(VAO::Vertex {glm::vec3(point.getX(), point.getY(), .0f)});
        component->_indices[VAO::IBO_POINT].push_back(vertexIdx);
    }

    this->_components.push_back(std::unique_ptr<Component>(component));
    this->buildVao(component);
}

GDSA::Render::DrawPointCloud::DrawPointCloud(Geometry::PointCloud3d& pointCloud)
    : Model3D()
{
    size_t     numPoints = pointCloud.size();
    Component* component = new Component;

    for(unsigned vertexIdx = 0; vertexIdx < numPoints; vertexIdx++)
    {
        Geometry::Vect3d point = pointCloud.getPoint(vertexIdx);
        component->_vertices.push_back(VAO::Vertex {glm::vec3(point.getX(), point.getY(), point.getZ())});
        component->_indices[VAO::IBO_POINT].push_back(vertexIdx);
    }

    this->_components.push_back(std::unique_ptr<Component>(component));
    this->buildVao(component);
}

GDSA::Render::DrawPointCloud::~DrawPointCloud()
{
}
