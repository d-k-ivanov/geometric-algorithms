#include "DrawLine.h"

namespace GDSA::Render
{
DrawLine::DrawLine(Geometry::Line& line)
    : Model3D()
{
    Component* component = new Component;

    Geometry::Point orig = line.getPoint(-100000);
    Geometry::Point dest = line.getPoint(100000);

    component->_vertices.insert(
        component->_vertices.end(),
        {VAO::Vertex {glm::vec3(orig.getX(), orig.getY(), .0f)},
         VAO::Vertex {glm::vec3(dest.getX(), dest.getY(), .0f)}});

    component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {0, 1});
    this->_components.push_back(std::unique_ptr<Component>(component));

    component->completeTopology();
    this->buildVao(component);
}

DrawLine::DrawLine(Geometry::Line3d& line)
    : Model3D()
{
    Component* component = new Component;

    const Geometry::Vect3d orig = line.getPoint(-100000);
    const Geometry::Vect3d dest = line.getPoint(100000);

    component->_vertices.insert(
        component->_vertices.end(),
        {VAO::Vertex {glm::vec3(orig.getX(), orig.getY(), orig.getZ())},
         VAO::Vertex {glm::vec3(dest.getX(), dest.getY(), dest.getZ())}});

    component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {0, 1});
    this->_components.push_back(std::unique_ptr<Component>(component));

    component->completeTopology();
    this->buildVao(component);
}
}    // namespace GDSA::Render
