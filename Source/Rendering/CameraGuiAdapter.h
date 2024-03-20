#pragma once

#include "Camera.h"
#include "InterfaceAdapter.h"

namespace GDSA::Render
{
class CameraGuiAdapter : public InterfaceAdapter
{
private:
    Camera* _camera;

public:
    CameraGuiAdapter()
        : _camera(nullptr)
    {
    }

    ~CameraGuiAdapter() override = default;

    void renderGuiObject() override;
    void setCamera(Camera* camera) { _camera = camera; }
};
}    // namespace GDSA::Render
