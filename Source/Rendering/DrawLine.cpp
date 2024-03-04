#include "StdAfx.h"

#include "DrawLine.h"

AlgGeom::DrawLine::DrawLine(Line& line)
    : Model3D()
    , _line(line)
{
    Component* component = new Component;

    Point orig = line.getPoint(INT_MAX);
    Point dest = line.getPoint(INT_MIN);

    component->_vertices.insert(component->_vertices.end(), {VAO::Vertex {vec3(orig.getX(), orig.getY(), .0f)}, VAO::Vertex {vec3(dest.getX(), dest.getY(), .0f)}});

    component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), {0, 1});
    this->_components.push_back(std::unique_ptr<Component>(component));

    component->completeTopology();
    this->buildVao(component);
}
