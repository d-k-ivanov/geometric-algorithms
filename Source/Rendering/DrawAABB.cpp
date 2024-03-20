#include "DrawAABB.h"

GDSA::Render::DrawAABB::DrawAABB(Geometry::AABB& aabb)
    : Model3D()
{
    Component* component = this->getVoxel();
    const glm::vec3  aabbCenter(aabb.getCenter().getX(), aabb.getCenter().getY(), aabb.getCenter().getZ());
    const glm::vec3  extent(aabb.getExtent().getX(), aabb.getExtent().getY(), aabb.getExtent().getZ());

    for(VAO::Vertex& vertex : component->_vertices)
    {
        vertex._position = (vertex._position * 2.0f * extent) + aabbCenter;
    }

    this->buildVao(component);
    this->_components.push_back(std::unique_ptr<Component>(component));
}

GDSA::Render::DrawAABB::~DrawAABB()
{
}
