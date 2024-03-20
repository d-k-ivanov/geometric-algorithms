#pragma once

#include "ShaderProgram.h"

namespace GDSA::Render
{
class RenderingShader : public ShaderProgram
{
public:
    RenderingShader()           = default;
    ~RenderingShader() override = default;
    void   applyActiveSubroutines() override;
    GLuint createShaderProgram(const char* filename) override;
};
}    // namespace GDSA::Render
