#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include <string>

namespace AlgGeom
{
namespace GuiUtilities
{
    void leaveSpace(unsigned numSlots);
    void renderText(const glm::vec3& xyz, const std::string& title = "", char delimiter = ',');
}
}    // namespace AlgGeom
