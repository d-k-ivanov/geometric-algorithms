#pragma once

#include "Image.h"

#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

namespace GDSA::Render
{
class FBO
{
protected:
    GLuint     _id;
    glm::ivec2 _size;

protected:
    void checkFBOstate() const;

public:
    FBO(uint16_t width, uint16_t height);
    virtual ~FBO();

    virtual GLuint getId() const { return _id; }
    glm::ivec2     getSize() const { return _size; }

    virtual void bindFBO() = 0;
    virtual void modifySize(uint16_t width, uint16_t height);

    void threadedWriteImage(std::vector<GLubyte>* pixels, const std::string& filename, uint16_t width, uint16_t height) const;
};
}    // namespace GDSA::Render
