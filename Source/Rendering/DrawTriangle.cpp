#include "DrawTriangle.h"

GDSA::DrawTriangle::DrawTriangle(Triangle& triangle)
    : Model3D()
{
    Component* component = new Component;
    component->_vertices.insert(
        component->_vertices.end(),
        {VAO::Vertex {glm::vec3(triangle.getA().getX(), triangle.getA().getY(), .0f), glm::vec3(.0f, .0f, 1.0f)},
         VAO::Vertex {glm::vec3(triangle.getB().getX(), triangle.getB().getY(), .0f), glm::vec3(.0f, .0f, 1.0f)},
         VAO::Vertex {glm::vec3(triangle.getC().getX(), triangle.getC().getY(), .0f), glm::vec3(.0f, .0f, 1.0f)}});

    component->_indices[VAO::IBO_TRIANGLE].insert(component->_indices[VAO::IBO_TRIANGLE].end(), {0, 1, 2, RESTART_PRIMITIVE_INDEX});
    component->completeTopology();
    this->_components.push_back(std::unique_ptr<Component>(component));

    this->buildVao(component);
}

GDSA::DrawTriangle::DrawTriangle(Triangle3d& triangle)
    : Model3D()
{
    Component* component = new Component;

    const Vect3d    normal    = triangle.normal();
    const glm::vec3 normalGlm = glm::vec3(normal.getX(), normal.getY(), normal.getZ());
    component->_vertices.insert(
        component->_vertices.end(),
        {VAO::Vertex {glm::vec3(triangle.getA().getX(), triangle.getA().getY(), triangle.getA().getZ()), normalGlm},
         VAO::Vertex {glm::vec3(triangle.getB().getX(), triangle.getB().getY(), triangle.getB().getZ()), normalGlm},
         VAO::Vertex {glm::vec3(triangle.getC().getX(), triangle.getC().getY(), triangle.getC().getZ()), normalGlm}});

    component->_indices[VAO::IBO_TRIANGLE].insert(component->_indices[VAO::IBO_TRIANGLE].end(), {0, 1, 2, RESTART_PRIMITIVE_INDEX});
    component->completeTopology();
    this->_components.push_back(std::unique_ptr<Component>(component));

    this->buildVao(component);
}
