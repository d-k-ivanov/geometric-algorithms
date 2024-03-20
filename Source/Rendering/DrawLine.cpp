#include "DrawLine.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

GDSA::DrawLine::DrawLine(Line& line)
    : Model3D()
{
    Component* component = new Component;

    Point orig = line.getPoint(-100000);
    Point dest = line.getPoint(100000);

    component->_vertices.insert(
        component->_vertices.end(),
        {VAO::Vertex {glm::vec3(orig.getX(), orig.getY(), .0f)},
         VAO::Vertex {glm::vec3(dest.getX(), dest.getY(), .0f)}});

    component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {0, 1});
    this->_components.push_back(std::unique_ptr<Component>(component));

    component->completeTopology();
    this->buildVao(component);
}

GDSA::DrawLine::DrawLine(Line3d& line)
    : Model3D()
{
    Component* component = new Component;

    const Vect3d orig = line.getPoint(-100000);
    const Vect3d dest = line.getPoint(100000);

    component->_vertices.insert(
        component->_vertices.end(),
        {VAO::Vertex {glm::vec3(orig.getX(), orig.getY(), orig.getZ())},
         VAO::Vertex {glm::vec3(dest.getX(), dest.getY(), dest.getZ())}});

    component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {0, 1});
    this->_components.push_back(std::unique_ptr<Component>(component));

    component->completeTopology();
    this->buildVao(component);
}