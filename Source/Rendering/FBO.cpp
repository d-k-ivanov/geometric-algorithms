#include "FBO.h"

#include <stdexcept>
#include <string>
#include <vector>

namespace GDSA::Render
{
FBO::FBO(const uint16_t width, const uint16_t height)
    : _id(0)
    , _size(width, height)
{
}

FBO::~FBO()
{
    glDeleteFramebuffers(1, &_id);
}

void FBO::modifySize(const uint16_t width, const uint16_t height)
{
    _size = glm::vec2(width, height);
}

void FBO::checkFBOstate() const
{
    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::runtime_error("FBO could not be created!");
    }
}

void FBO::threadedWriteImage(std::vector<GLubyte>* pixels, const std::string& filename, const uint16_t width, const uint16_t height) const
{
    Image* image = new Image(pixels->data(), width, height, 4);
    image->saveImage(filename);
    delete pixels;
    delete image;
}
}    // namespace GDSA::Render
