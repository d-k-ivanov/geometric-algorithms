#pragma once

#include "ShaderProgram.h"

namespace GDSA
{
class RenderingShader : public ShaderProgram
{
public:
    RenderingShader();
    ~RenderingShader() override;
    void   applyActiveSubroutines() override;
    GLuint createShaderProgram(const char* filename) override;
};
}    // namespace GDSA
