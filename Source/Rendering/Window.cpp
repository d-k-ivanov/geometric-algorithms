#include "Window.h"

#include "InputManager.h"

#include <iostream>
#include <stdexcept>

AlgGeom::Window::Window()
    : _window(nullptr)
{
    _renderer = Renderer::getInstance();
}

void AlgGeom::Window::releaseResources()
{
    glfwDestroyWindow(_window);    // We close and destroy the application window.
    _window = nullptr;
    glfwTerminate();    // We freed up the resources that GLFW was occupying.
}

void AlgGeom::Window::init(const std::string& title, const uint16_t width, const uint16_t height)
{
    // Initialize GLFW. It is a process that only needs to be done once in the application
    if(glfwInit() != GLFW_TRUE)
    {
        throw std::runtime_error("Failed to initialize GLFW!");
    }

    // We define the characteristics that we want the OpenGL graphical context of the window that we are going to create to have.
    // For example, the number of samples or the Core Profile mode.
    glfwWindowHint(GLFW_SAMPLES, 4);                                  // - Activa antialiasing con 4 muestras.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    // - Esta y las 2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                    // siguientes activan un contexto
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);                    // OpenGL Core Profile 4.1.

    // Size, window title, in window and not in full screen, without sharing resources with other windows.
    _window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    // We check if the creation of the window has been successful.
    if(_window == nullptr)
    {
        glfwTerminate();    // We freed up the resources that GLFW was occupying.
        throw std::runtime_error("Failed to open GLFW window!");
    }

    // Makes the OpenGL context associated with the window we just created become the current OpenGL context for subsequent library calls
    glfwMakeContextCurrent(_window);

    // IMPORTANT: GLEW must always be initialized AFTER GLFW has been initialized and the window created
    glewExperimental = true;
    if(glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(_window);
        glfwTerminate();    // - Liberamos los recursos que ocupaba GLFW.
        throw std::runtime_error("Failed to initialize GLEW!");
    }

    // We interrogate OpenGL to inform us of the properties of the constructed 3D context.
    std::cout << glGetString(GL_RENDERER) << '\n'
              << glGetString(GL_VENDOR) << '\n'
              << glGetString(GL_VERSION) << '\n'
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

    InputManager::getInstance()->init(_window);
    Renderer::getInstance()->prepareOpenGL(width, height, InputManager::getApplicationState());
    GUI::getInstance()->initialize(_window, 2);
}

void AlgGeom::Window::loop()
{
    while(!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();

        InputManager::getInstance()->windowRefreshCallback(_window);

        glfwSwapBuffers(_window);
    }

    this->releaseResources();
}
