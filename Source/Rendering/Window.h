#pragma once

#include "Renderer.h"

#include "Patterns/Singleton.h"

namespace GDSA
{
class Window : public Singleton<Window>
{
    friend class Singleton<Window>;

private:
    Renderer*   _renderer;
    GLFWwindow* _window;

private:
    Window();
    void releaseResources();

public:
    virtual ~Window() = default;
    void init(const std::string& title, const uint16_t width = 1856, const uint16_t height = 1392);
    void loop();
};
}    // namespace GDSA
