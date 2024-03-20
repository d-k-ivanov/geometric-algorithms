#include "DrawRay.h"

GDSA::Render::DrawRay::DrawRay(Geometry::RayLine& ray)
    : Model3D()
{
    Component* component = new Component;
    Geometry::Point dest      = ray.getPoint(100000);

    component->_vertices.insert(
        component->_vertices.end(),
        {VAO::Vertex {glm::vec3(ray.getA().getX(), ray.getA().getY(), .0f)},
         VAO::Vertex {glm::vec3(dest.getX(), dest.getY(), .0f)}});

    component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {0, 1});
    this->_components.push_back(std::unique_ptr<Component>(component));

    component->completeTopology();
    this->buildVao(component);
}

GDSA::Render::DrawRay::DrawRay(Geometry::Ray3d& ray)
    : Model3D()
{
    Component*   component = new Component;
    const Geometry::Vect3d dest      = ray.getPoint(100000);

    component->_vertices.insert(
        component->_vertices.end(),
        {VAO::Vertex {glm::vec3(ray.getOrigin().getX(), ray.getOrigin().getY(), ray.getOrigin().getZ())},
         VAO::Vertex {glm::vec3(dest.getX(), dest.getY(), dest.getZ())}});

    component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {0, 1});
    this->_components.push_back(std::unique_ptr<Component>(component));

    component->completeTopology();
    this->buildVao(component);
}
