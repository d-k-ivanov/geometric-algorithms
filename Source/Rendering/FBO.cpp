#include "FBO.h"

#include <stdexcept>
#include <string>
#include <vector>

GDSA::Render::FBO::FBO(const uint16_t width, const uint16_t height)
    : _id(0)
    , _size(width, height)
{
}

GDSA::Render::FBO::~FBO()
{
    glDeleteFramebuffers(1, &_id);
}

void GDSA::Render::FBO::modifySize(const uint16_t width, const uint16_t height)
{
    _size = glm::vec2(width, height);
}

void GDSA::Render::FBO::checkFBOstate()
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::runtime_error("FBO could not be created!");
    }
}

void GDSA::Render::FBO::threadedWriteImage(std::vector<GLubyte>* pixels, const std::string& filename, const uint16_t width, const uint16_t height)
{
    Image* image = new Image(pixels->data(), width, height, 4);
    image->saveImage(filename);
    delete pixels;
    delete image;
}
