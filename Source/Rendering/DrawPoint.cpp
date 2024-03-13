#include "DrawPoint.h"

AlgGeom::DrawPoint::DrawPoint(const Point& point)
    : Model3D()
    , _point(point)
{
    Component* component = new Component;
    component->_vertices.insert(component->_vertices.end(), {VAO::Vertex {glm::vec3(_point.getX(), _point.getY(), .0f)}});
    component->_indices[VAO::IBO_POINT].insert(component->_indices[VAO::IBO_POINT].end(), {0});
    this->_components.push_back(std::unique_ptr<Component>(component));

    this->buildVao(component);
}

AlgGeom::DrawPoint::DrawPoint(const Vect3d& point)
    : Model3D()
{
    Component* component = new Component;
    component->_vertices.insert(component->_vertices.end(), {VAO::Vertex {glm::vec3(point.getX(), point.getY(), point.getZ())}});
    component->_indices[VAO::IBO_POINT].insert(component->_indices[VAO::IBO_POINT].end(), {0});
    this->_components.push_back(std::unique_ptr<Component>(component));

    this->buildVao(component);
}
