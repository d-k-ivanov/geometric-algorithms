#include "DrawCircle.h"

GDSA::Render::DrawCircle::DrawCircle(const Geometry::Circle& circle, const uint32_t numSubdivisions, bool drawTriangularSegments)
    : Model3D()
    , _circle(circle)
    , _numSubdivisions(numSubdivisions)
{
    Component* component = new Component;

    Geometry::Polygon polygon     = circle.getPointsCircle(numSubdivisions);
    const size_t      numVertices = polygon.getNumVertices();

    for(unsigned vertexIdx = 0; vertexIdx < polygon.getNumVertices(); vertexIdx++)
    {
        Geometry::Point point = polygon.getVertexAt(vertexIdx).getPoint();
        component->_vertices.push_back(VAO::Vertex {glm::vec3(point.getX(), point.getY(), .0f), glm::vec3(.0f, .0f, 1.0f)});
    }
    component->_vertices.push_back(VAO::Vertex {glm::vec3(.0f, .0f, .0f)});

    for(unsigned vertexIdx = 0; vertexIdx < polygon.getNumVertices(); vertexIdx++)
    {
        if(drawTriangularSegments)
        {
            component->_indices[VAO::IBO_TRIANGLE].insert(
                component->_indices[VAO::IBO_TRIANGLE].end(),
                {static_cast<unsigned>(component->_vertices.size()) - 1, vertexIdx, (vertexIdx + 1) % numVertices, RESTART_PRIMITIVE_INDEX});
        }
        component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {vertexIdx, (vertexIdx + 1) % numVertices, RESTART_PRIMITIVE_INDEX});
    }

    component->completeTopology();
    this->_components.push_back(std::unique_ptr<Component>(component));
    this->buildVao(component);
}
