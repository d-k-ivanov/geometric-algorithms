#pragma once

#include "Model3D.h"

#include "Geometry/AABB.h"

namespace GDSA::Render
{
class DrawAABB : public Model3D
{
public:
    DrawAABB(Geometry::AABB& aabb);
    DrawAABB(const DrawAABB& aabb) = delete;
    ~DrawAABB() override           = default;
};
}
