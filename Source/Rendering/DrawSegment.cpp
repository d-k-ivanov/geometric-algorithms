#include "DrawSegment.h"

GDSA::Render::DrawSegment::DrawSegment(Geometry::SegmentLine& segment)
    : Model3D()
{
    Component* component = new Component;
    component->_vertices.insert(
        component->_vertices.end(),
        {VAO::Vertex {glm::vec3(segment.getA().getX(), segment.getA().getY(), .0f)},
         VAO::Vertex {glm::vec3(segment.getB().getX(), segment.getB().getY(), .0f)}});

    component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {0, 1});
    this->_components.push_back(std::unique_ptr<Component>(component));

    component->completeTopology();
    this->buildVao(component);
}

GDSA::Render::DrawSegment::DrawSegment(Geometry::Segment3d& segment)
    : Model3D()
{
    Component* component = new Component;
    component->_vertices.insert(
        component->_vertices.end(),
        {VAO::Vertex {glm::vec3(segment.getOrigin().getX(), segment.getOrigin().getY(), segment.getOrigin().getZ())},
         VAO::Vertex {glm::vec3(segment.getDestination().getX(), segment.getDestination().getY(), segment.getDestination().getZ())}});

    component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {0, 1});
    this->_components.push_back(std::unique_ptr<Component>(component));

    component->completeTopology();
    this->buildVao(component);
}
