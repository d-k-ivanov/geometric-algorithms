#include "StdAfx.h"

#include "Window.h"

#include "InputManager.h"

AlgGeom::Window::Window()
    : _window(nullptr)
{
    _renderer = Renderer::getInstance();
}

void AlgGeom::Window::releaseResources()
{
    glfwDestroyWindow(_window);    // - Cerramos y destruimos la ventana de la aplicaci�n.
    _window = nullptr;
    glfwTerminate();    // - Liberamos los recursos que ocupaba GLFW.
}

AlgGeom::Window::~Window()
{
}

void AlgGeom::Window::init(const std::string& title, const uint16_t width, const uint16_t height)
{
    // - Inicializa GLFW. Es un proceso que s�lo debe realizarse una vez en la aplicaci�n
    if(glfwInit() != GLFW_TRUE)
    {
        throw std::runtime_error("Failed to initialize GLFW!");
    }

    // - Definimos las caracter�sticas que queremos que tenga el contexto gr�fico
    // OpenGL de la ventana que vamos a crear. Por ejemplo, el n�mero de muestras o el
    // modo Core Profile.
    glfwWindowHint(GLFW_SAMPLES, 4);                                  // - Activa antialiasing con 4 muestras.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    // - Esta y las 2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                    // siguientes activan un contexto
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);                    // OpenGL Core Profile 4.1.

    // - Tama�o, t�tulo de la ventana, en ventana y no en pantalla completa, sin compartir recursos con otras ventanas.
    _window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    // - Comprobamos si la creaci�n de la ventana ha tenido �xito.
    if(_window == nullptr)
    {
        glfwTerminate();    // - Liberamos los recursos que ocupaba GLFW.
        throw std::runtime_error("Failed to open GLFW window!");
    }

    // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent(_window);

    // - Ahora inicializamos GLEW.
    // IMPORTANTE: GLEW debe inicializarse siempre DESPU�S de que se haya
    // inicializado GLFW y creado la ventana
    glewExperimental = true;
    if(glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(_window);
        glfwTerminate();    // - Liberamos los recursos que ocupaba GLFW.
        throw std::runtime_error("Failed to initialize GLEW!");
    }

    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto 3D construido.
    std::cout << glGetString(GL_RENDERER) << '\n'
              << glGetString(GL_VENDOR) << '\n'
              << glGetString(GL_VERSION) << '\n'
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

    InputManager::getInstance()->init(_window);
    Renderer::getInstance()->prepareOpenGL(width, height, InputManager::getApplicationState());
    GUI::getInstance()->initialize(_window, 5);
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
