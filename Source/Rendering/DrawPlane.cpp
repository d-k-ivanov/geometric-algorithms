#include "DrawPlane.h"

#include "Geometry/Triangle3d.h"

AlgGeom::DrawPlane::DrawPlane(Plane& plane)
    : Model3D()
    , _plane(plane)
{
    Component*          component = new Component;
    std::vector<Vect3d> vertices;
    constexpr size_t    numVertices = 2;
    constexpr double    planeSize   = 1.5;
    constexpr double    planeScale  = 10.5;

    // Version 1:
    // Line3d*    line1     = new Line3d(plane.get_A(), plane.get_B());
    // Line3d*    line2     = new Line3d(plane.get_A(), plane.get_C());
    // Line3d*    line3     = new Line3d(plane.get_B(), plane.get_C());

    // Version 2:

    // Vect3d  p1(0, 0, -plane.getD() / plane.getC());
    // Vect3d  p2(0, plane.getB(), (-plane.getD() - plane.getB()) / plane.getC());
    // Vect3d  p3(0, -plane.getD() / plane.getB(), 0);
    // Line3d* line1 = new Line3d(p1, p2);
    // Line3d* line2 = new Line3d(p1, p3);
    // Line3d* line3 = new Line3d(p2, p3);

    // vertices.push_back(line1->getPoint(planeSize));
    // vertices.push_back(line2->getPoint(planeSize));
    // vertices.push_back(line3->getPoint(planeSize));
    // vertices.push_back(line1->getPoint(-planeSize));
    // vertices.push_back(line2->getPoint(-planeSize));
    // vertices.push_back(line3->getPoint(-planeSize));

    // Version 3:
    Vect3d center = {0, 0, 0};
    Vect3d t      = plane.get_B().sub(plane.get_A()).normalize();
    Vect3d b      = t.xProduct(plane.getNormal());

    Vect3d v1 = plane.get_A().sub(t.scalarMul(planeScale)).sub(b.scalarMul(planeScale));
    Vect3d v2 = plane.get_A().add(t.scalarMul(planeScale)).sub(b.scalarMul(planeScale));
    Vect3d v3 = plane.get_A().add(t.scalarMul(planeScale)).add(b.scalarMul(planeScale));
    Vect3d v4 = plane.get_A().sub(t.scalarMul(planeScale)).add(b.scalarMul(planeScale));
    // Vect3d v1 = plane.get_A().sub(t.scalarMul(plane.getD())).sub(b.scalarMul(plane.getD()));
    // Vect3d v2 = plane.get_A().add(t.scalarMul(plane.getD())).sub(b.scalarMul(plane.getD()));
    // Vect3d v3 = plane.get_A().add(t.scalarMul(plane.getD())).add(b.scalarMul(plane.getD()));
    // Vect3d v4 = plane.get_A().sub(t.scalarMul(plane.getD())).add(b.scalarMul(plane.getD()));

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    for(unsigned vertexIdx = 0; vertexIdx < vertices.size(); vertexIdx++)
    {
        Vect3d point = vertices[vertexIdx];
        component->_vertices.push_back(VAO::Vertex {glm::vec3(point.getX(), point.getY(), point.getZ())});
        component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {vertexIdx, static_cast<unsigned int>((vertexIdx + 1) % vertices.size()), RESTART_PRIMITIVE_INDEX});
    }

    this->_components.push_back(std::unique_ptr<Component>(component));
    this->buildVao(component);
}

AlgGeom::DrawPlane::~DrawPlane()
{
}
