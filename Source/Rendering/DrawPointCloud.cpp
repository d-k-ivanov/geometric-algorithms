#include "DrawPointCloud.h"

#include <execution>

namespace GDSA::Render
{
DrawPointCloud::DrawPointCloud(Geometry::PointCloud& pointCloud)
    : Model3D()
{
    Component* component = new Component;
    for(unsigned vertexIdx = 0; vertexIdx < pointCloud.size(); vertexIdx++)
    {
        component->_vertices.emplace_back(pointCloud.getPoint(vertexIdx).toGlmVec3(), glm::vec3(), glm::vec3());
        component->_indices[VAO::IBO_POINT].push_back(vertexIdx);
    }

    this->_components.push_back(std::unique_ptr<Component>(component));
    this->buildVao(component);
}

DrawPointCloud::DrawPointCloud(Geometry::PointCloud3d& pointCloud, const bool drawSegmentsToCenter)
    : Model3D()
{
    if(drawSegmentsToCenter)
    {
        for(auto& point : pointCloud.getPoints())
        {
            Component* component = new Component;
            component->_vertices.insert(component->_vertices.end(), {VAO::Vertex {glm::vec3(), glm::vec3(), glm::vec3()}, VAO::Vertex {point.toGlmVec3(), glm::vec3(), glm::vec3()}});
            component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {0, 1});
            component->completeTopology();
            this->_components.push_back(std::unique_ptr<Component>(component));
            this->buildVao(component);
        }
    }
    else
    {
        Component* component = new Component;
        for(unsigned vertexIdx = 0; vertexIdx < pointCloud.size(); vertexIdx++)
        {

            component->_vertices.emplace_back(pointCloud.getPoint(vertexIdx).toGlmVec3(), glm::vec3(), glm::vec3());
            component->_indices[VAO::IBO_POINT].push_back(vertexIdx);
        }
        // component->completeTopology();
        this->_components.push_back(std::unique_ptr<Component>(component));
        this->buildVao(component);
    }
}
}    // namespace GDSA::Render
