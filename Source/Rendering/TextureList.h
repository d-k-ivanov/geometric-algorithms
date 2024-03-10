#pragma once

#include "Texture.h"

#include "Patterns/Singleton.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <unordered_map>

namespace AlgGeom
{
class TextureList : public Singleton<TextureList>
{
    friend class Singleton<TextureList>;

private:
    struct ColorHash
    {
        std::size_t operator()(const glm::vec4& color) const
        {
            glm::ivec4 iColor = color * 256.0f;
            return static_cast<size_t>(iColor.x + iColor.y * 256 + iColor.z * 512 + iColor.w + 1024);
        }
    };

    struct ColorEqual
    {
        bool operator()(const glm::vec4& c1, const glm::vec4& c2) const
        {
            return glm::distance(c1, c2) < glm::epsilon<float>();
        }
    };

private:
    std::unordered_map<glm::vec4, AlgGeom::Texture*, ColorHash, ColorEqual> _colorTexture;
    std::unordered_map<std::string, AlgGeom::Texture*>                 _imageTexture;

private:
    TextureList();

public:
    virtual ~TextureList();

    AlgGeom::Texture* getTexture(const glm::vec4& color);
    AlgGeom::Texture* getTexture(const std::string& path);
    void              saveTexture(const glm::vec4& color, AlgGeom::Texture* texture);
    void              saveTexture(const std::string& path, AlgGeom::Texture* texture);
};
}    // namespace AlgGeom
