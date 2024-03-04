#include "StdAfx.h"

#include "DrawPointCloud.h"

AlgGeom::DrawPointCloud::DrawPointCloud(PointCloud& pointCloud)
    : Model3D()
    , _pointCloud(pointCloud)
{
    Component* component = new Component;

    for(unsigned vertexIdx = 0; vertexIdx < pointCloud.size(); vertexIdx++)
    {
        Point point = pointCloud.getPoint(vertexIdx);
        component->_vertices.push_back(VAO::Vertex {vec3(point.getX(), point.getY(), .0f)});
        component->_indices[VAO::IBO_POINT].push_back(vertexIdx);
    }

    this->_components.push_back(std::unique_ptr<Component>(component));
    this->buildVao(component);
}
