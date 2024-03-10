#pragma once

#include "Texture.h"
#include "VAO.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

namespace AlgGeom
{
struct ApplicationState
{
    // Application
    glm::vec3     _backgroundColor;
    float    _materialScattering;
    uint16_t _numFps;
    uint8_t  _selectedCamera;
    glm::ivec2    _viewportSize;

    // Screenshot
    char  _screenshotFilenameBuffer[60];
    float _screenshotFactor;
    bool  _transparentScreenshot;

    // Ligthing
    glm::vec3  _lightPosition, _Ia, _Id, _Is;
    float _gamma;

    // Topology
    bool _activeRendering[VAO::NUM_IBOS];

    ApplicationState()
    {
        _backgroundColor    = glm::vec3(.6f);
        _materialScattering = 1.0f;
        _numFps             = 0;
        _selectedCamera     = 0;
        _viewportSize       = glm::vec3(0);

        std::strcpy(_screenshotFilenameBuffer, "ScreenshotRGBA.png");
        _screenshotFactor = 3.0f;

        for(int i = 0; i < VAO::NUM_IBOS; ++i)
            _activeRendering[i] = /*i == VAO::IBO_TRIANGLE*/ true;

        _lightPosition = glm::vec3(.0f);
        _Ia            = glm::vec3(.6f);
        _Id            = glm::vec3(1.0f);
        _Is            = glm::vec3(1.0f);
        _gamma         = 1.3f;
    }
};
}    // namespace AlgGeom
