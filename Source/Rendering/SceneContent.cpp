#include "SceneContent.h"
#include "Scenes.h"

#include "DrawBezier.h"
#include "DrawLine.h"
#include "DrawPoint.h"
#include "DrawPointCloud.h"
#include "DrawPolygon.h"
#include "DrawRay.h"
#include "DrawSegment.h"
#include "Geometry/Bezier.h"

#include "Geometry/PointCloud.h"
#include "Geometry/Polygon.h"
#include "Geometry/SegmentLine.h"

#include "Utils/ChronoUtilities.h"
#include "Utils/RandomUtilities.h"

#include <iostream>

// ----------------------------- BUILD YOUR SCENARIO HERE -----------------------------------

void AlgGeom::SceneContent::buildScenario()
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);

    // Define the scene
    // Scenes::p0(*this);

    // Practice 1:
    constexpr int      numPointClouds = 0;
    constexpr int      pointsPerCloud = 100;
    constexpr float    scaleFactor    = 1.0f;
    std::vector<Point> randomPointsFromCloud;
    std::vector<Point> extremumPointInCloud;

    // Scenes::p1PointClouds(*this, numPointClouds, pointsPerCloud, scaleFactor, randomPointsFromCloud, extremumPointInCloud);
    // Scenes::p1Lines(*this, randomPointsFromCloud);
    // Scenes::p1Polygon(*this, extremumPointInCloud);
    // Scenes::p1Bezier(*this);
    // Scenes::p1Bezier(*this, true, 5);
    // Scenes::p1Intersections(*this);
    Scenes::p1All(*this);
}

void AlgGeom::SceneContent::buildCamera(uint16_t width, uint16_t height)
{
    Camera* camera = new Camera(width, height, true);
    camera->setPosition(glm::vec3(.0f, .0f, 4.0f));
    camera->setLookAt(glm::vec3(.0f, .0f, 0.0f));
    camera->saveCamera();

    this->_camera.push_back(std::unique_ptr<Camera>(camera));
}

// ------------------------------------------------------------------------------------------

AlgGeom::SceneContent::SceneContent()
{
}

AlgGeom::SceneContent::~SceneContent()
{
    _camera.clear();
    _model.clear();
}

void AlgGeom::SceneContent::addNewCamera(ApplicationState* appState)
{
    _camera.push_back(std::make_unique<Camera>(appState->_viewportSize.x, appState->_viewportSize.y, true));
}

void AlgGeom::SceneContent::addNewModel(Model3D* model)
{
    _model.push_back(std::unique_ptr<Model3D>(model));
}

AlgGeom::Model3D* AlgGeom::SceneContent::getModel(Model3D::Component* component)
{
    for(auto& model : _model)
    {
        if(model->belongsModel(component))
            return model.get();
    }

    return nullptr;
}
