#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace AlgGeom
{
class CameraProjection
{
protected:
    static std::vector<std::shared_ptr<CameraProjection>> _cameraProjection;

public:
    enum Projection
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    class CameraProperties
    {
    public:
        int  _cameraType;
        bool _2d;

        glm::vec3 _eye, _lookAt, _up;
        float     _zNear, _zFar;
        float     _aspect;
        float     _fovY, _fovX;
        glm::vec2 _bottomLeftCorner;
        uint16_t  _width, _height;
        glm::vec3 _n, _u, _v;
        glm::mat4 _viewMatrix, _projectionMatrix, _viewProjectionMatrix;

        float     computeAspect();
        void      computeAxes(glm::vec3& n, glm::vec3& u, glm::vec3& v);
        glm::vec2 computeBottomLeftCorner();
        float     computeFovY();

        void computeProjectionMatrices(CameraProperties* camera);
        void computeViewMatrices();
        void computeViewMatrix();

        void zoom(float speed);
    };

public:
    virtual glm::mat4 buildProjectionMatrix(CameraProperties* camera)   = 0;
    virtual void      zoom(CameraProperties* camera, const float speed) = 0;
};

class PerspectiveProjection : public CameraProjection
{
public:
    virtual glm::mat4 buildProjectionMatrix(CameraProperties* camera);
    virtual void      zoom(CameraProperties* camera, const float speed);
};

class OrthographicProjection : public CameraProjection
{
public:
    virtual glm::mat4 buildProjectionMatrix(CameraProperties* camera);
    virtual void      zoom(CameraProperties* camera, const float speed);
};
}
