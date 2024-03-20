#include "DrawPolygon.h"

namespace GDSA::Render
{
DrawPolygon::DrawPolygon(Geometry::Polygon& polygon)
    : Model3D()
    , _polygon(polygon)
{
    const size_t numVertices = _polygon.getNumVertices();
    Component*   component   = new Component;

    for(unsigned vertexIdx = 0; vertexIdx < _polygon.getNumVertices(); vertexIdx++)
    {
        Geometry::Point point = _polygon.getVertexAt(vertexIdx).getPoint();
        component->_vertices.push_back(VAO::Vertex {glm::vec3(point.getX(), point.getY(), .0f)});
        component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {vertexIdx, static_cast<unsigned int>((vertexIdx + 1) % numVertices), RESTART_PRIMITIVE_INDEX});
    }

    this->_components.push_back(std::unique_ptr<Component>(component));
    this->buildVao(component);
}
}    // namespace GDSA::Render
