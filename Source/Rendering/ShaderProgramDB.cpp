#include "ShaderProgramDB.h"

#include "Utils/FilesystemUtilities.h"

namespace GDSA::Render
{
std::unordered_map<uint8_t, std::string> ShaderProgramDB::RENDERING_SHADER_PATH {
    {static_cast<int>(RenderingShaderId::LINE_RENDERING), Utils::ThisExecutableLocation() + "/Resources/Shaders/line"},
    {static_cast<int>(RenderingShaderId::POINT_RENDERING), Utils::ThisExecutableLocation() + "/Resources/Shaders/point"},
    {static_cast<int>(RenderingShaderId::TRIANGLE_RENDERING), Utils::ThisExecutableLocation() + "/Resources/Shaders/triangle"},
};

std::unordered_map<uint8_t, std::unique_ptr<RenderingShader>> ShaderProgramDB::_renderingShader;

RenderingShader* ShaderProgramDB::getShader(const RenderingShaderId shaderId) const
{
    const uint8_t shaderId8 = static_cast<uint8_t>(shaderId);

    if(!_renderingShader[shaderId8].get())
    {
        RenderingShader* shader = new RenderingShader();
        shader->createShaderProgram(RENDERING_SHADER_PATH.at(shaderId8).c_str());

        _renderingShader[shaderId8].reset(shader);
    }

    return _renderingShader[shaderId8].get();
}
}    // namespace GDSA::Render
