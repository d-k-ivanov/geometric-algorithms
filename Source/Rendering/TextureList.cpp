#include "TextureList.h"

#include <stdexcept>

GDSA::TextureList::TextureList()
{
}

GDSA::TextureList::~TextureList()
{
    for(auto& pair : _colorTexture)
    {
        delete pair.second;
    }

    for(auto& pair : _imageTexture)
    {
        delete pair.second;
    }
}

GDSA::Texture* GDSA::TextureList::getTexture(const glm::vec4& color)
{
    GDSA::Texture* texture = nullptr;
    auto              it      = _colorTexture.find(color);

    if(it == _colorTexture.end())
    {
        texture              = new GDSA::Texture(color);
        _colorTexture[color] = texture;
    }
    else
        texture = it->second;

    return texture;
}

GDSA::Texture* GDSA::TextureList::getTexture(const std::string& path)
{
    GDSA::Texture* texture = nullptr;
    auto              it      = _imageTexture.find(path);

    if(it == _imageTexture.end())
    {
        try
        {
            texture             = new GDSA::Texture(new Image(path));
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

void GDSA::TextureList::saveTexture(const glm::vec4& color, GDSA::Texture* texture)
{
    _colorTexture[color] = texture;
}

void GDSA::TextureList::saveTexture(const std::string& path, GDSA::Texture* texture)
{
    _imageTexture[path] = texture;
}
