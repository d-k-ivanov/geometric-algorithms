#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

#include <iostream>

int main()
{
    AlgGeom::Window*   window   = AlgGeom::Window::getInstance();
    AlgGeom::Renderer* renderer = AlgGeom::Renderer::getInstance();

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
