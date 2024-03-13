#pragma once

#include "Model3D.h"

#include "Geometry/AABB.h"

namespace AlgGeom
{
class DrawAABB : public Model3D
{
public:
    DrawAABB(AABB& aabb);
    DrawAABB(const DrawAABB& dt) = delete;
    ~DrawAABB() override;
};
}
