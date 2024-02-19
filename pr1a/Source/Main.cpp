#include "StdAfx.h"

#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

int main()
{
    AlgGeom::Window* window = AlgGeom::Window::getInstance();
    AlgGeom::Renderer* renderer = AlgGeom::Renderer::getInstance();

    try
    {
        window->init("Geometric Algorithms");
        window->loop();
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << '\n';
    }

    // Once the event cycle is finished, release resources, etc.
    std::cout << "Finishing application...\n";

    // This call is to prevent the console from closing immediately after the execution and being able to read the messages.
    // You can also use getChar();
    system("pause");
}

