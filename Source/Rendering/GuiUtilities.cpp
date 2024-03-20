#include "GuiUtilities.h"

#include <imgui.h>

void GDSA::Render::GuiUtilities::leaveSpace(unsigned numSlots)
{
    for(unsigned i = 0; i < numSlots; ++i)
        ImGui::Spacing();
}

void GDSA::Render::GuiUtilities::renderText(const glm::vec3& xyz, const std::string& title, char delimiter)
{
    const std::string txt = title + (title.empty() ? "" : ": ") + std::to_string(xyz.x) + delimiter + ' ' + std::to_string(xyz.y) + delimiter + ' ' + std::to_string(xyz.z);
    ImGui::Text("%s", txt.c_str());
}
