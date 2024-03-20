#pragma once

#include <string>
#include <vector>

namespace GDSA::Render
{
class Image
{
protected:
    std::string                _filename;
    std::vector<unsigned char> _image;
    unsigned                   _width;
    unsigned                   _height;
    unsigned                   _depth;

public:
    Image(const std::string& filename);
    Image(unsigned char* image, uint16_t width, uint16_t height, uint8_t depth);
    ~Image() = default;

    void        flipImageVertically();
    static void flipImageVertically(std::vector<unsigned char>& image, const uint16_t width, const uint16_t height, const uint8_t depth);
    bool        saveImage(const std::string& filename) const;

    unsigned char* bits() { return _image.data(); }
    unsigned       getDepth() const { return _depth; }
    std::string    getFilename() { return _filename; }
    unsigned       getHeight() const { return _height; }
    unsigned       getWidth() const { return _width; }
};
}    // namespace GDSA::Render
