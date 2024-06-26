#include "GUI.h"

#include "DrawMesh.h"
#include "Fonts/Lato.h"
#include "GuiUtilities.h"
#include "InputManager.h"

#include "Fonts/FontAwesome.h"
#include "Fonts/IconsFontAwesome5.h"

#include "Libraries/ImGuiFileDialog/ImGuiFileDialog.h"

#include "Utils/FilesystemUtilities.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>
#include <string>

namespace GDSA::Render
{
GUI::GUI()
{
    _appState              = InputManager::getApplicationState();
    _cameraGuiAdapter      = new CameraGuiAdapter;
    _currentGizmoOperation = ImGuizmo::TRANSLATE;
    _currentGizmoMode      = ImGuizmo::WORLD;
    _fileDialog            = FileDialog::NONE;
    _lastDirectory         = DEFAULT_DIRECTORY;
    _modelCompSelected     = nullptr;

    _showMenuButtons = new bool[MenuButtons::NUM_GUI_MENU_BUTTONS];
    for(int idx = 0; idx < MenuButtons::NUM_GUI_MENU_BUTTONS; ++idx)
        _showMenuButtons[idx] = false;
}

GUI::~GUI()
{
    delete[] _showMenuButtons;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::editTransform(ImGuizmo::OPERATION& operation, ImGuizmo::MODE& mode) const
{
    if(ImGui::RadioButton("Translate", operation == ImGuizmo::TRANSLATE))
    {
        operation = ImGuizmo::TRANSLATE;
    }

    ImGui::SameLine();
    if(ImGui::RadioButton("Rotate", operation == ImGuizmo::ROTATE))
    {
        operation = ImGuizmo::ROTATE;
    }

    ImGui::SameLine();
    if(ImGui::RadioButton("Scale", operation == ImGuizmo::SCALE))
    {
        operation = ImGuizmo::SCALE;
    }

    if(operation != ImGuizmo::SCALE)
    {
        if(ImGui::RadioButton("Local", mode == ImGuizmo::LOCAL))
        {
            mode = ImGuizmo::LOCAL;
        }

        ImGui::SameLine();
        if(ImGui::RadioButton("World", mode == ImGuizmo::WORLD))
        {
            mode = ImGuizmo::WORLD;
        }
    }
}

void GUI::loadFonts() const
{
    ImFontConfig cfg;
    ImGuiIO&     io = ImGui::GetIO();

    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;

    std::copy_n("Lato", 5, cfg.Name);
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(LatoFont::lato_compressed_data_base85, 13.0f, &cfg);

    static constexpr ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    cfg.MergeMode                           = true;
    cfg.PixelSnapH                          = true;
    cfg.GlyphMinAdvanceX                    = 20.0f;
    cfg.GlyphMaxAdvanceX                    = 20.0f;
    std::copy_n("FontAwesome", 12, cfg.Name);

    io.Fonts->AddFontFromFileTTF((Utils::ThisExecutableLocation() + "/Resources/Fonts/fa-regular-400.ttf").c_str(), 12.0f, &cfg, icons_ranges);
    io.Fonts->AddFontFromFileTTF((Utils::ThisExecutableLocation() + "/Resources/Fonts/fa-solid-900.ttf").c_str(), 12.0f, &cfg, icons_ranges);
}

void GUI::loadImGUIStyle() const
{
    ImGui::StyleColorsLight();
    this->loadFonts();
}

void GUI::processSelectedFile(const FileDialog fileDialog, const std::string& filename, SceneContent* sceneContent) const
{
    if(fileDialog == FileDialog::OPEN_MESH)
    {
        // TriangleModel* triangleModel = new TriangleModel(filename);
        Model3D* model = (new DrawMesh())->loadModelOBJ(filename);
        // Model3D* model = (new DrawMesh(*triangleModel))->overrideModelName();
        // model->moveGeometryToOrigin();
        sceneContent->addNewModel(model);

        // delete triangleModel;
    }
}

void GUI::renderGuizmo(Model3D::Component* component, const SceneContent* sceneContent)
{
    if(component && _showMenuButtons[MenuButtons::MODELS])
    {
        if(ImGui::IsKeyPressed(ImGuiKey_T))
            _currentGizmoOperation = ImGuizmo::TRANSLATE;

        if(ImGui::IsKeyPressed(ImGuiKey_R))
            _currentGizmoOperation = ImGuizmo::ROTATE;

        if(ImGui::IsKeyPressed(ImGuiKey_S))
            _currentGizmoOperation = ImGuizmo::SCALE;

        const glm::mat4 viewMatrix       = glm::mat4(sceneContent->_camera[_appState->_selectedCamera]->getViewMatrix());
        const glm::mat4 projectionMatrix = glm::mat4(sceneContent->_camera[_appState->_selectedCamera]->getProjectionMatrix());
        Model3D*        model            = sceneContent->getModel(component);

        if(model)
        {
            glm::mat4 modelMatrix = model->getModelMatrix();

            const ImGuiIO& io = ImGui::GetIO();
            ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
            Manipulate(&(viewMatrix[0][0]), &(projectionMatrix[0][0]), _currentGizmoOperation, _currentGizmoMode, &(modelMatrix[0][0]), nullptr, nullptr);

            model->setModelMatrix(modelMatrix);
        }
    }
}

void GUI::initialize(GLFWwindow* window, const int openGLMinorVersion) const
{
    const std::string openGLVersion = "#version 4" + std::to_string(openGLMinorVersion) + "0 core";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    this->loadImGUIStyle();

    // Add Windows icon to GLFW window
    GLFWimage images[1];
    images[0].pixels = stbi_load((Utils::ThisExecutableLocation() + "/Resources/App.png").c_str(), &images[0].width, &images[0].height, nullptr, 4);
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(openGLVersion.c_str());
}

void GUI::render(SceneContent* sceneContent)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    for(int menuButtonIdx = 0; menuButtonIdx < MenuButtons::NUM_GUI_MENU_BUTTONS; ++menuButtonIdx)
    {
        const MenuButtons button = static_cast<MenuButtons>(menuButtonIdx);

        if(_showMenuButtons[button])
        {
            switch(button)
            {
                case MenuButtons::RENDERING:
                    this->showRenderingMenu(sceneContent);
                    break;
                case MenuButtons::MODELS:
                    this->showModelMenu(sceneContent);
                    break;
                case MenuButtons::CAMERA:
                    this->showCameraMenu(sceneContent);
                    break;
                case MenuButtons::LIGHT:
                    this->showLightMenu(sceneContent);
                    break;
                case MenuButtons::SCREENSHOT:
                    this->showScreenshotMenu(sceneContent);
                    break;
                case MenuButtons::NUM_GUI_MENU_BUTTONS:
                    break;
            }
        }
    }

    this->showFileDialog(sceneContent);

    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu(ICON_FA_COG "Settings"))
        {
            ImGui::MenuItem(ICON_FA_DRAW_POLYGON "Rendering", nullptr, &_showMenuButtons[MenuButtons::RENDERING]);
            ImGui::MenuItem(ICON_FA_CUBE "Models", nullptr, &_showMenuButtons[MenuButtons::MODELS]);
            ImGui::MenuItem(ICON_FA_CAMERA_RETRO "Camera", nullptr, &_showMenuButtons[MenuButtons::CAMERA]);
            ImGui::MenuItem(ICON_FA_LIGHTBULB "Light", nullptr, &_showMenuButtons[MenuButtons::LIGHT]);
            ImGui::MenuItem(ICON_FA_CAMERA "Screenshot", nullptr, &_showMenuButtons[MenuButtons::SCREENSHOT]);
            ImGui::EndMenu();
        }

        ImGui::SameLine(0, 20);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::EndMainMenuBar();
    }

    this->renderGuizmo(_modelCompSelected, sceneContent);

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::showCameraMenu(const SceneContent* sceneContent) const
{
    static Camera* cameraSelected = nullptr;

    if(ImGui::Begin("Lights", &this->_showMenuButtons[MenuButtons::CAMERA], ImGuiWindowFlags_None))
    {
        GuiUtilities::leaveSpace(2);
        ImGui::BeginChild("Camera List", ImVec2(200, 0), true);

        for(int cameraIdx = 0; cameraIdx < sceneContent->_camera.size(); ++cameraIdx)
        {
            const std::string cameraName = "Camera " + std::to_string(cameraIdx);
            if(ImGui::Selectable(cameraName.c_str(), cameraSelected == sceneContent->_camera[cameraIdx].get()))
            {
                cameraSelected             = sceneContent->_camera[cameraIdx].get();
                _appState->_selectedCamera = cameraIdx;
            }
        }

        ImGui::EndChild();
        ImGui::SameLine();

        ImGui::BeginGroup();
        ImGui::BeginChild("Model Component View", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));    // Leave room for 1 line below us

        if(cameraSelected)
        {
            _cameraGuiAdapter->setCamera(cameraSelected);
            _cameraGuiAdapter->renderGuiObject();
        }

        ImGui::EndChild();
        ImGui::EndGroup();

        ImGui::End();
    }
}

void GUI::showFileDialog(SceneContent* sceneContent)
{
    if(_fileDialog != FileDialog::NONE)
    {
        if(std::filesystem::exists(_lastDirectory))
            _lastDirectory = DEFAULT_DIRECTORY;

        const uint16_t iFileDialog = static_cast<uint16_t>(_fileDialog) / 2;
        ImGuiFileDialog::Instance()->OpenDialog(FILE_DIALOG_TEXT[iFileDialog], "Select a file", FILE_DIALOG_EXTENSION[iFileDialog].c_str(), static_cast<IGFD::FileDialogConfig>(_lastDirectory));

        if(ImGuiFileDialog::Instance()->Display(FILE_DIALOG_TEXT[iFileDialog]))
        {
            if(ImGuiFileDialog::Instance()->IsOk())
            {
                _path          = ImGuiFileDialog::Instance()->GetFilePathName();
                _lastDirectory = _path.substr(0, _path.find_last_of("\\"));

                this->processSelectedFile(_fileDialog, _path, sceneContent);
                _fileDialog = FileDialog::NONE;
            }

            ImGuiFileDialog::Instance()->Close();
            _fileDialog = FileDialog::NONE;
        }
    }
}

void GUI::showLightMenu(SceneContent* sceneContent) const
{
    ImGui::SetNextWindowSize(ImVec2(800, 440), ImGuiCond_FirstUseEver);

    if(ImGui::Begin("Lights", &this->_showMenuButtons[MenuButtons::LIGHT], ImGuiWindowFlags_None))
    {
        GuiUtilities::leaveSpace(1);
        ImGui::Text("Light Properties");
        ImGui::Separator();
        GuiUtilities::leaveSpace(2);

        ImGui::SliderFloat3("Light Position", &_appState->_lightPosition[0], -10.0f, 10.0f);
        ImGui::ColorEdit3("Ia", &_appState->_Ia[0]);
        ImGui::ColorEdit3("Id", &_appState->_Id[0]);
        ImGui::ColorEdit3("Is", &_appState->_Is[0]);

        ImGui::End();
    }
}

void GUI::showModelMenu(const SceneContent* sceneContent)
{
    ImGui::SetNextWindowSize(ImVec2(800, 440), ImGuiCond_FirstUseEver);

    if(ImGui::Begin("Models", &this->_showMenuButtons[MenuButtons::MODELS], ImGuiWindowFlags_None))
    {
        if(ImGui::Button("Open Model"))
        {
            _fileDialog = FileDialog::OPEN_MESH;
        }

        GuiUtilities::leaveSpace(2);
        ImGui::BeginChild("Components", ImVec2(200, 0), true);

        unsigned globalIdx = 0;

        for(size_t modelIdx = 0; modelIdx < sceneContent->_model.size(); ++modelIdx)
        {
            for(size_t compIdx = 0; compIdx < sceneContent->_model[modelIdx]->_components.size(); ++compIdx)
            {
                const std::string compName = sceneContent->_model[modelIdx]->getName() + ", " + "Comp. " + std::to_string(compIdx);
                if(ImGui::Selectable(compName.c_str(), _modelCompSelected == sceneContent->_model[modelIdx]->_components[compIdx].get()))
                {
                    _modelCompSelected = sceneContent->_model[modelIdx]->_components[compIdx].get();
                }
            }
        }

        ImGui::EndChild();
        ImGui::SameLine();

        ImGui::BeginGroup();
        ImGui::BeginChild("Model Component View", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));    // Leave room for 1 line below us

        if(_modelCompSelected)
        {
            ImGui::Text("Visibility");
            ImGui::Separator();

            ImGui::Checkbox("Enabled", &_modelCompSelected->_enabled);

            GuiUtilities::leaveSpace(4);
            ImGui::Text("Material");
            ImGui::Separator();

            ImGui::Checkbox("Use Uniform Color", &_modelCompSelected->_material._useUniformColor);
            ImGui::ColorEdit4("Kd Color", &_modelCompSelected->_material._kdColor[0]);
            ImGui::ColorEdit3("Ks Color", &_modelCompSelected->_material._ksColor[0]);
            ImGui::SliderFloat("Metallic", &_modelCompSelected->_material._metallic, .0f, 1.0f);
            ImGui::SliderFloat("Roughness exponent", &_modelCompSelected->_material._roughnessK, .0f, 1.0f);
            ImGui::ColorEdit3("Line Color", &_modelCompSelected->_material._lineColor[0]);
            ImGui::ColorEdit3("Point Color", &_modelCompSelected->_material._pointColor[0]);

            GuiUtilities::leaveSpace(2);
            ImGui::Text("Topology");
            ImGui::Separator();

            static const char* topologyTitle[] = {"Point Cloud", "Wireframe", "Triangle Mesh"};
            for(int topologyIdx = 0; topologyIdx < VAO::NUM_IBOS; ++topologyIdx)
            {
                ImGui::Checkbox(topologyTitle[topologyIdx], &_modelCompSelected->_activeRendering[topologyIdx]);
                if(topologyIdx < VAO::NUM_IBOS - 1)
                    ImGui::SameLine(0, 10);
            }
            ImGui::SliderFloat("Line Width", &_modelCompSelected->_lineWidth, .0f, 10.0f);
            ImGui::SliderFloat("Point Size", &_modelCompSelected->_pointSize, .0f, 10.0f);

            GuiUtilities::leaveSpace(2);

            ImGui::Text("Matrices");
            ImGui::Separator();

            this->editTransform(_currentGizmoOperation, _currentGizmoMode);
        }

        ImGui::EndChild();
        ImGui::EndGroup();
    }

    ImGui::End();
}

void GUI::showRenderingMenu(SceneContent* sceneContent) const
{
    if(ImGui::Begin("Rendering Settings", &_showMenuButtons[MenuButtons::RENDERING]))
    {
        ImGui::ColorEdit3("Background color", &_appState->_backgroundColor[0]);
        ImGui::SliderFloat("Gamma", &_appState->_gamma, 1.0f, 5.0f);

        GuiUtilities::leaveSpace(3);

        if(ImGui::BeginTabBar("Rendering Tab Bar"))
        {
            if(ImGui::BeginTabItem("Texture"))
            {
                GuiUtilities::leaveSpace(1);

                ImGui::Text(ICON_FA_NETWORK_WIRED "Topology");
                ImGui::Separator();

                static const char* topologyTitle[] = {"Point Cloud", "Wireframe", "Triangle Mesh"};
                for(int topologyIdx = 0; topologyIdx < VAO::NUM_IBOS; ++topologyIdx)
                {
                    ImGui::Checkbox(topologyTitle[topologyIdx], &_appState->_activeRendering[topologyIdx]);
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    ImGui::End();
}

void GUI::showScreenshotMenu(SceneContent* sceneContent) const
{
    auto fixName = [=](const std::string& name, const std::string& defaultName, const std::string& extension) -> std::string
    {
        if(name.empty())
            return defaultName + extension;
        else if(name.find(extension) == std::string::npos)
            return name + extension;

        return name;
    };

    if(ImGui::Begin("Screenshot Settings", &_showMenuButtons[MenuButtons::SCREENSHOT]))
    {
        ImGui::SliderFloat("Size multiplier", &_appState->_screenshotFactor, 1.0f, 10.0f);
        ImGui::SameLine(0, 20);
        ImGui::Checkbox("Transparent", &_appState->_transparentScreenshot);
        ImGui::InputText("Filename (RGB)", _appState->_screenshotFilenameBuffer, IM_ARRAYSIZE(_appState->_screenshotFilenameBuffer));

        GuiUtilities::leaveSpace(2);

        if(ImGui::Button("Take screenshot (RGBA)"))
        {
            std::string filename = _appState->_screenshotFilenameBuffer;
            filename             = fixName(filename, "RGB", ".png");
            InputManager::getInstance()->pushScreenshotEvent(ScreenshotListener::ScreenshotEvent {ScreenshotListener::RGBA});
        }
    }

    ImGui::End();
}
}    // namespace GDSA::Render
