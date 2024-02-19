#pragma once

#include "StdAfx.h"

#include "ApplicationState.h"
#include "Camera.h"
#include "Model3D.h"

namespace AlgGeom
{
class SceneContent
{
public:
    std::vector<std::unique_ptr<Camera>>  _camera;
    std::vector<std::unique_ptr<Model3D>> _model;

    SceneContent();
    virtual ~SceneContent();

    void     addNewCamera(ApplicationState* appState);
    void     addNewModel(Model3D* model);
    void     buildCamera(uint16_t width, uint16_t height);
    void     buildScenario();
    Model3D* getModel(Model3D::Component* component);
};
}    // namespace AlgGeom