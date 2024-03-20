#pragma once

#include "ShaderProgram.h"

namespace GDSA::Render
{
class RenderingShader : public ShaderProgram
{
public:
    RenderingShader();
    ~RenderingShader() override;
    void   applyActiveSubroutines() override;
    GLuint createShaderProgram(const char* filename) override;
};
}    // namespace GDSA::Render
