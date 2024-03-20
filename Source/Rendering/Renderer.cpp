#include "Renderer.h"

#include "InputManager.h"
#include "ShaderProgramDB.h"

#include "Utils/ChronoUtilities.h"

#include <glm/ext/matrix_transform.hpp>

GDSA::Render::Renderer::Renderer()
    : _appState(nullptr)
    , _content(nullptr)
    , _screenshoter(nullptr)
    , _triangleShader(nullptr)
    , _lineShader(nullptr)
    , _pointShader(nullptr)
{
    _gui = GUI::getInstance();
}

void GDSA::Render::Renderer::renderLine(Model3D::MatrixRenderInformation* matrixInformation)
{
    _lineShader->use();

    for(auto& model : _content->_model)
    {
        model->draw(_lineShader, matrixInformation, _appState, GL_LINES);
    }
}

void GDSA::Render::Renderer::renderPoint(Model3D::MatrixRenderInformation* matrixInformation)
{
    _pointShader->use();

    for(auto& model : _content->_model)
    {
        model->draw(_pointShader, matrixInformation, _appState, GL_POINTS);
    }
}

void GDSA::Render::Renderer::renderTriangle(Model3D::MatrixRenderInformation* matrixInformation)
{
    _triangleShader->use();
    this->transferLightUniforms(_triangleShader);
    _triangleShader->setUniform("gamma", _appState->_gamma);

    for(auto& model : _content->_model)
    {
        model->draw(_triangleShader, matrixInformation, _appState, GL_TRIANGLES);
    }
}

void GDSA::Render::Renderer::transferLightUniforms(RenderingShader* shader)
{
    shader->setUniform("lightPosition", _appState->_lightPosition);
    shader->setUniform("Ia", _appState->_Ia);
    shader->setUniform("Id", _appState->_Id);
    shader->setUniform("Is", _appState->_Is);
}

GDSA::Render::Renderer::~Renderer()
{
    delete _screenshoter;
}

void GDSA::Render::Renderer::createCamera(uint16_t width, uint16_t height)
{
    _content->buildCamera(width, height);
}

void GDSA::Render::Renderer::createModels()
{
    _content->buildScenario();
}

void GDSA::Render::Renderer::createShaderProgram()
{
    _pointShader    = ShaderProgramDB::getInstance()->getShader(ShaderProgramDB::POINT_RENDERING);
    _lineShader     = ShaderProgramDB::getInstance()->getShader(ShaderProgramDB::LINE_RENDERING);
    _triangleShader = ShaderProgramDB::getInstance()->getShader(ShaderProgramDB::TRIANGLE_RENDERING);
}

void GDSA::Render::Renderer::prepareOpenGL(uint16_t width, uint16_t height, ApplicationState* appState)
{
    _appState                = appState;
    _appState->_viewportSize = glm::ivec2(width, height);
    _content                 = new SceneContent {};
    _screenshoter            = new FBOScreenshot(width, height);

    // We set a medium gray as the color with which the frame buffer will be cleared.
    // It does not have to be executed at every step through the event loop.
    glClearColor(_appState->_backgroundColor.x, _appState->_backgroundColor.y, _appState->_backgroundColor.z, 1.0f);

    // We tell OpenGL to take depth into account when drawing.
    // It does not have to be executed at every step through the event loop.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(RESTART_PRIMITIVE_INDEX);

    glEnable(GL_PROGRAM_POINT_SIZE);

    glEnable(GL_POLYGON_OFFSET_FILL);

    this->createShaderProgram();
    this->createModels();
    this->createCamera(width, height);

    // Observer
    InputManager* inputManager = InputManager::getInstance();
    inputManager->suscribeResize(this);
    inputManager->suscribeScreenshot(this);

    this->resizeEvent(_appState->_viewportSize.x, _appState->_viewportSize.y);
}

void GDSA::Render::Renderer::removeModel()
{
    if(!_content->_model.empty())
        _content->_model.erase(_content->_model.end() - 1);
}

void GDSA::Render::Renderer::resizeEvent(uint16_t width, uint16_t height)
{
    glViewport(0, 0, width, height);

    _appState->_viewportSize = glm::ivec2(width, height);
    _content->_camera[_appState->_selectedCamera]->setRaspect(width, height);
}

void GDSA::Render::Renderer::screenshotEvent(const ScreenshotEvent& event)
{
    if(event._type == ScreenshotListener::RGBA)
    {
        const glm::ivec2 size    = _appState->_viewportSize;
        const glm::ivec2 newSize = glm::ivec2(_appState->_viewportSize.x * _appState->_screenshotFactor, _appState->_viewportSize.y * _appState->_screenshotFactor);

        this->resizeEvent(newSize.x, newSize.y);
        this->render(_appState->_transparentScreenshot ? .0f : 1.0f, false, true);
        _screenshoter->saveImage(_appState->_screenshotFilenameBuffer);

        this->resizeEvent(size.x, size.y);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void GDSA::Render::Renderer::render(float alpha, bool renderGui, bool bindScreenshoter)
{
    Model3D::MatrixRenderInformation matrixInformation;
    glm::mat4                        bias = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    matrixInformation.setMatrix(Model3D::MatrixRenderInformation::VIEW, _content->_camera[0]->getViewMatrix());
    matrixInformation.setMatrix(Model3D::MatrixRenderInformation::VIEW_PROJECTION, _content->_camera[0]->getViewProjectionMatrix());

    if(bindScreenshoter)
    {
        _screenshoter->modifySize(_appState->_viewportSize.x, _appState->_viewportSize.y);
        _screenshoter->bindFBO();
    }

    glClearColor(_appState->_backgroundColor.x, _appState->_backgroundColor.y, _appState->_backgroundColor.z, alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonOffset(1.0f, 1.0f);

    if(_appState->_activeRendering[VAO::IBO_TRIANGLE])
    {
        this->renderTriangle(&matrixInformation);
    }

    if(_appState->_activeRendering[VAO::IBO_LINE])
    {
        this->renderLine(&matrixInformation);
    }

    if(_appState->_activeRendering[VAO::IBO_POINT])
    {
        this->renderPoint(&matrixInformation);
    }

    glPolygonOffset(.0f, .0f);

    if(renderGui)
        _gui->render(_content);

    _appState->_numFps = _gui->getFrameRate();
}
