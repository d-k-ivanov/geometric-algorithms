#include "Image.h"

#include "lodepng.h"

#include <iostream>

namespace GDSA::Render
{
Image::Image(const std::string& filename)
    : _depth(4)    // PNG depth
{
    const unsigned error = lodepng::decode(_image, _width, _height, filename);

    if(error)
    {
        std::cout << "The image " << filename << " couldn't be loaded by lodepng\n";

        _width = _height = _depth = 0;
    }

    this->flipImageVertically(_image, static_cast<uint16_t>(_width), static_cast<uint16_t>(_height), static_cast<uint8_t>(_depth));    // By default it's flipped
}

Image::Image(unsigned char* image, const uint16_t width, const uint16_t height, const uint8_t depth)
    : _width(width)
    , _height(height)
    , _depth(depth)
{
    if(image)
    {
        const int size = width * height * depth;
        _image         = std::vector<unsigned char>(image, image + size);
    }
    else
    {
        std::cout << "Empty image!\n";

        _width = _height = _depth = 0;
    }
}

void Image::flipImageVertically(std::vector<unsigned char>& image, const uint16_t width, const uint16_t height, const uint8_t depth)
{
    const int      rowSize    = width * depth;
    unsigned char* bits       = image.data();
    unsigned char* tempBuffer = new unsigned char[rowSize];

    for(int i = 0; i < (height / 2); ++i)
    {
        unsigned char* source = bits + i * rowSize;
        unsigned char* target = bits + (height - i - 1) * rowSize;

        memcpy(tempBuffer, source, rowSize);    // Swap with help of temporary buffer
        memcpy(source, target, rowSize);
        memcpy(target, tempBuffer, rowSize);
    }

    delete[] tempBuffer;
}

bool Image::saveImage(const std::string& filename) const
{
    std::vector<unsigned char> result;
    const unsigned             error = lodepng::encode(result, this->_image, this->_width, this->_height);

    if(!error)
    {
        lodepng::save_file(result, filename);
        return true;
    }

    return false;
}

void Image::flipImageVertically()
{
    flipImageVertically(_image, static_cast<uint16_t>(_width), static_cast<uint16_t>(_height), static_cast<uint8_t>(_depth));
}
}    // namespace GDSA::Render
