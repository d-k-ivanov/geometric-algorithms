#include "TextureList.h"

#include <stdexcept>

namespace GDSA::Render
{
TextureList::TextureList()
{
}

TextureList::~TextureList()
{
    for(const auto& pair : _colorTexture)
    {
        delete pair.second;
    }

    for(const auto& pair : _imageTexture)
    {
        delete pair.second;
    }
}

Texture* TextureList::getTexture(const glm::vec4& color)
{
    Texture*   texture = nullptr;
    const auto it      = _colorTexture.find(color);

    if(it == _colorTexture.end())
    {
        texture              = new Texture(color);
        _colorTexture[color] = texture;
    }
    else
        texture = it->second;

    return texture;
}

Texture* TextureList::getTexture(const std::string& path)
{
    Texture*   texture = nullptr;
    const auto it      = _imageTexture.find(path);

    if(it == _imageTexture.end())
    {
        try
        {
            texture             = new Texture(new Image(path));
            _imageTexture[path] = texture;
        }
        catch(std::runtime_error& error)
        {
            return nullptr;
        }
    }
    else
    {
        texture = it->second;
    }

    return texture;
}

void TextureList::saveTexture(const glm::vec4& color, Texture* texture)
{
    _colorTexture[color] = texture;
}

void TextureList::saveTexture(const std::string& path, Texture* texture)
{
    _imageTexture[path] = texture;
}
}    // namespace GDSA::Render
