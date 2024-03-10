#include "ShaderProgramDB.h"

#include "Utils/FilesystemUtilities.h"

std::unordered_map<uint8_t, std::string> AlgGeom::ShaderProgramDB::RENDERING_SHADER_PATH {
    {RenderingShaderId::LINE_RENDERING, ThisExecutableLocation() + "/Resources/Shaders/line"},
    {RenderingShaderId::POINT_RENDERING, ThisExecutableLocation() + "/Resources/Shaders/point"},
    {RenderingShaderId::TRIANGLE_RENDERING, ThisExecutableLocation() + "/Resources/Shaders/triangle"},
};

std::unordered_map<uint8_t, std::unique_ptr<AlgGeom::RenderingShader>> AlgGeom::ShaderProgramDB::_renderingShader;

AlgGeom::ShaderProgramDB::ShaderProgramDB()
{
}

AlgGeom::ShaderProgramDB::~ShaderProgramDB()
{
}

AlgGeom::RenderingShader* AlgGeom::ShaderProgramDB::getShader(const RenderingShaderId shaderId)
{
    uint8_t shaderId8 = static_cast<uint8_t>(shaderId);

    if(!_renderingShader[shaderId8].get())
    {
        RenderingShader* shader = new RenderingShader();
        shader->createShaderProgram(RENDERING_SHADER_PATH.at(shaderId8).c_str());

        _renderingShader[shaderId8].reset(shader);
    }

    return _renderingShader[shaderId8].get();
}
