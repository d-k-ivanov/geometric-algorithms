#include "DrawBezier.h"

#include "Geometry/Line.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

namespace GDSA::Render
{
DrawBezier::DrawBezier(const Geometry::Bezier& bezier, const double lertStep = 0.02f)
    : _bezier(bezier)
{
    for(double t = 0; t < 1; t += lertStep)
    {
        Component*       component = new Component;
        Geometry::Vect2d point     = _bezier.getPoint(t);

        // component->_vertices.push_back(VAO::Vertex {vec3(point.getX(), point.getY(), .0f)});
        // component->_indices[VAO::IBO_POINT].push_back(component->_indices[VAO::IBO_POINT].size());
        // component->_indices[VAO::IBO_POINT].push_back(t);

        component->_vertices.insert(component->_vertices.end(), {VAO::Vertex {glm::vec3(point.getX(), point.getY(), .0f)}});
        component->_indices[VAO::IBO_POINT].insert(component->_indices[VAO::IBO_POINT].end(), {0});
        this->_components.push_back(std::unique_ptr<Component>(component));
        this->buildVao(component);
    }

    // this->_components.push_back(std::unique_ptr<Component>(component));
}
}    // namespace GDSA::Render