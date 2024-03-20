#include "TextureList.h"

#include <stdexcept>

GDSA::Render::TextureList::TextureList()
{
}

GDSA::Render::TextureList::~TextureList()
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

GDSA::Render::Texture* GDSA::Render::TextureList::getTexture(const glm::vec4& color)
{
    GDSA::Render::Texture* texture = nullptr;
    auto              it      = _colorTexture.find(color);

    if(it == _colorTexture.end())
    {
        texture              = new GDSA::Render::Texture(color);
        _colorTexture[color] = texture;
    }
    else
        texture = it->second;

    return texture;
}

GDSA::Render::Texture* GDSA::Render::TextureList::getTexture(const std::string& path)
{
    GDSA::Render::Texture* texture = nullptr;
    auto              it      = _imageTexture.find(path);

    if(it == _imageTexture.end())
    {
        try
        {
            texture             = new GDSA::Render::Texture(new Image(path));
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

void GDSA::Render::TextureList::saveTexture(const glm::vec4& color, GDSA::Render::Texture* texture)
{
    _colorTexture[color] = texture;
}

void GDSA::Render::TextureList::saveTexture(const std::string& path, GDSA::Render::Texture* texture)
{
    _imageTexture[path] = texture;
}
