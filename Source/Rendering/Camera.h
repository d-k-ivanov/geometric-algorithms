#pragma once

#include "CameraProjection.h"
#include "Model3D.h"

#include "Geometry/AABB.h"

namespace AlgGeom
{
class Camera
{
    friend class CameraGuiAdapter;

protected:
    Camera*                            _backupCamera;
    CameraProjection::CameraProperties _properties;

protected:
    void copyCameraAttributes(const Camera* camera);

public:
    Camera(uint16_t width, uint16_t height, bool is2D = false);
    Camera(const Camera& camera);
    virtual ~Camera();
    void reset();
    void track(AABB& aabb);

    Camera& operator=(const Camera& camera) = delete;

    glm::mat4 getProjectionMatrix() const { return _properties._projectionMatrix; }
    glm::mat4 getViewMatrix() const { return _properties._viewMatrix; }
    glm::mat4 getViewProjectionMatrix() const { return _properties._viewProjectionMatrix; }

    void saveCamera();
    void setBottomLeftCorner(const glm::vec2& bottomLeft);
    void setCameraType(CameraProjection::Projection projection);
    void setFovX(float fovX);
    void setFovY(float fovY);
    void setLookAt(const glm::vec3& position);
    void setPosition(const glm::vec3& position);
    void setRaspect(uint16_t width, uint16_t height);
    void setUp(const glm::vec3& up);
    void setZFar(float zfar);
    void setZNear(float znear);
    void updateMatrices();

    // Movements

    void boom(float speed);
    void crane(float speed);
    void dolly(float speed);
    void orbitXZ(float speed);
    void orbitY(float speed);
    void pan(float speed);
    void tilt(float speed);
    void truck(float speed);
    void zoom(float speed);
};
}    // namespace AlgGeom
