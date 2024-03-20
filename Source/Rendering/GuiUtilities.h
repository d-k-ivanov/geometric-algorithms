#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include <string>

namespace GDSA
{
namespace GuiUtilities
{
    void leaveSpace(unsigned numSlots);
    void renderText(const glm::vec3& xyz, const std::string& title = "", char delimiter = ',');
}
}    // namespace GDSA
