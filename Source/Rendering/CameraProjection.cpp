#include "CameraProjection.h"

#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/epsilon.hpp>

#include <memory>

std::vector<std::shared_ptr<AlgGeom::CameraProjection>> AlgGeom::CameraProjection::_cameraProjection {
    std::make_shared<AlgGeom::PerspectiveProjection>(),
    std::make_shared<AlgGeom::OrthographicProjection>()};

float AlgGeom::CameraProjection::CameraProperties::computeAspect()
{
    return static_cast<float>(_width) / static_cast<float>(_height);
}

void AlgGeom::CameraProjection::CameraProperties::computeAxes(glm::vec3& n, glm::vec3& u, glm::vec3& v)
{
    n = glm::normalize(_eye - _lookAt);    // z axis

    if(glm::all(glm::epsilonEqual(n, -_up, glm::epsilon<float>())))    // x axis: UP x n is 0 as both vectors are parallel. Since up and n are normalized we can check if they are equal (with epsilon checkup)
    {
        u = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), n));
    }
    else if(glm::all(glm::epsilonEqual(n, _up, glm::epsilon<float>())))
    {
        u = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), n));
    }
    else
    {
        u = glm::normalize(glm::cross(_up, n));
    }
    v = glm::normalize(glm::cross(n, u));    // y axis
}

glm::vec2 AlgGeom::CameraProjection::CameraProperties::computeBottomLeftCorner()
{
    const float halfWidth  = _width / 2.0f;
    const float halfHeight = _height / 2.0f;

    return glm::vec2(-halfWidth, -halfHeight);
}

float AlgGeom::CameraProjection::CameraProperties::computeFovY()
{
    return 2.0f * glm::atan(glm::tan(_fovX / 2.0f) / _aspect);
}

void AlgGeom::CameraProjection::CameraProperties::computeProjectionMatrices(CameraProperties* camera)
{
    _projectionMatrix     = _cameraProjection[_cameraType]->buildProjectionMatrix(camera);
    _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
}

void AlgGeom::CameraProjection::CameraProperties::computeViewMatrices()
{
    this->computeViewMatrix();
    _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
}

void AlgGeom::CameraProjection::CameraProperties::computeViewMatrix()
{
    _viewMatrix = glm::lookAt(_eye, _lookAt, _up);
}

void AlgGeom::CameraProjection::CameraProperties::zoom(float speed)
{
    CameraProjection::_cameraProjection[this->_cameraType]->zoom(this, speed);
}

// Projection

glm::mat4 AlgGeom::PerspectiveProjection::buildProjectionMatrix(CameraProperties* camera)
{
    return glm::perspective(camera->_fovY, camera->_aspect, camera->_zNear, camera->_zFar);
}

void AlgGeom::PerspectiveProjection::zoom(CameraProperties* camera, const float speed)
{
    float angle = camera->_fovY - speed;
    if(angle < glm::pi<float>() && angle > 0.0f)
    {
        camera->_fovY = angle;
        camera->computeProjectionMatrices(camera);
    }
}

glm::mat4 AlgGeom::OrthographicProjection::buildProjectionMatrix(CameraProperties* camera)
{
    const glm::vec2 bottomLeftCorner = camera->_bottomLeftCorner;
    return glm::ortho(bottomLeftCorner.x, -bottomLeftCorner.x, bottomLeftCorner.y, -bottomLeftCorner.y, camera->_zNear, camera->_zFar);
}

void AlgGeom::OrthographicProjection::zoom(CameraProperties* camera, const float speed)
{
    const float units            = -speed;
    const float raspect          = camera->_aspect;
    glm::vec2   bottomLeftCorner = camera->_bottomLeftCorner;

    if(bottomLeftCorner.x - units >= 0 || bottomLeftCorner.y - units >= 0)
        return;

    bottomLeftCorner -= glm::vec2(units * raspect, units);
    camera->_bottomLeftCorner = bottomLeftCorner;

    camera->computeProjectionMatrices(camera);
}
