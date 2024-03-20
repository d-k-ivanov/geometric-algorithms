#pragma once

#include "RenderingShader.h"

#include "Patterns/Singleton.h"

namespace GDSA::Render
{
class ShaderProgramDB : public Singleton<ShaderProgramDB>
{
    friend class Singleton<ShaderProgramDB>;

private:
    static std::unordered_map<uint8_t, std::string>                      RENDERING_SHADER_PATH;
    static std::unordered_map<uint8_t, std::unique_ptr<RenderingShader>> _renderingShader;

public:
    enum RenderingShaderId
    {
        POINT_RENDERING,
        LINE_RENDERING,
        TRIANGLE_RENDERING,
    };

private:
    ShaderProgramDB() = default;

public:
    virtual ~ShaderProgramDB() = default;
    RenderingShader* getShader(RenderingShaderId shaderId) const;
};
}    // namespace GDSA::Render
