#include "DrawPlane.h"

AlgGeom::DrawPlane::DrawPlane(Plane& plane)
    : Model3D()
    , _plane(plane)
{
    size_t              numVertices = 6;
    std::vector<Vect3d> vertices;
    Component*          component = new Component;
    Vect3d              point1(0, 0, -plane.getD() / plane.getC());
    Vect3d              point2(0, plane.getB(), (-plane.getD() - plane.getB()) / plane.getC());
    Vect3d              point3(0, -plane.getD() / plane.getB(), 0);
    Line3d*             line1 = new Line3d(point1, point2);
    Line3d*             line2 = new Line3d(point1, point3);
    Line3d*             line3 = new Line3d(point2, point3);

    vertices.push_back(line1->getPoint(10));
    vertices.push_back(line2->getPoint(10));
    vertices.push_back(line3->getPoint(10));
    vertices.push_back(line1->getPoint(-10));
    vertices.push_back(line2->getPoint(-10));
    vertices.push_back(line3->getPoint(-10));

    for(unsigned vertexIdx = 0; vertexIdx < numVertices; vertexIdx++)
    {
        Vect3d point = vertices[vertexIdx];
        component->_vertices.push_back(VAO::Vertex {glm::vec3(point.getX(), point.getY(), point.getZ())});
        component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {vertexIdx, static_cast<unsigned int>((vertexIdx + 1) % numVertices), RESTART_PRIMITIVE_INDEX});
    }

    this->_components.push_back(std::unique_ptr<Component>(component));
    this->buildVao(component);
}

AlgGeom::DrawPlane::~DrawPlane()
{
}
