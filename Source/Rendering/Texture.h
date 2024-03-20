#pragma once

#include "Image.h"
#include "ShaderProgram.h"

#include <assimp/scene.h>

#include <GL/glew.h>

namespace GDSA
{
class Texture
{
protected:
    const static GLuint MIN_FILTER;
    const static GLuint MAG_FILTER;
    const static GLuint WRAP_S;
    const static GLuint WRAP_T;
    const static GLuint WRAP_R;

protected:
    GLuint      _id;
    glm::vec4   _color;
    std::string _filename;

public:
    Texture(Image* image, const GLuint wrapS = WRAP_S, const GLuint wrapT = WRAP_T, const GLuint minFilter = MIN_FILTER, const GLuint magFilter = MAG_FILTER);
    Texture(const glm::vec4& color);
    virtual ~Texture();

    void applyTexture(GDSA::ShaderProgram* shader, const GLint id, const std::string& shaderVariable);

    glm::vec4   getColor() { return _color; }
    std::string getFilename() { return _filename; }
};
}    // namespace GDSA
