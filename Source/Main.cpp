#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

#include <iostream>

int main()
{
    GDSA::Window*   window   = GDSA::Window::getInstance();
    GDSA::Renderer* renderer = GDSA::Renderer::getInstance();

    try
    {
        window->init("Geometric Algorithms");
        window->loop();
    }
    catch(const std::exception& exception)
    {
        std::cout << exception.what() << '\n';
    }

    std::cout << "Finishing application...\n";

    return 0;
}
