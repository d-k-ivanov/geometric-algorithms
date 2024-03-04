#include "StdAfx.h"

#include "DrawBezier.h"

AlgGeom::DrawBezier::DrawBezier(Bezier& bezier)
    : Model3D()
    , _bezier(bezier)
{
    Component* component = new Component;
    component->_vertices.reserve(_bezier.getControlPoints().size());
    for(auto& controlPoint : _bezier.getControlPoints())
    {
        component->_vertices.push_back(VAO::Vertex {vec3(controlPoint.getX(), controlPoint.getY(), .0f)});
        component->_indices[VAO::IBO_POINT].push_back(component->_indices[VAO::IBO_POINT].size());
    }

    this->_components.push_back(std::unique_ptr<Component>(component));
    this->buildVao(component);
}
