#pragma once

#include "Model3D.h"

#include "Geometry/AABB.h"

namespace GDSA
{
class DrawAABB : public Model3D
{
public:
    DrawAABB(AABB& aabb);
    DrawAABB(const DrawAABB& aabb) = delete;
    ~DrawAABB() override;
};
}
