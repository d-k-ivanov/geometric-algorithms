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

void GDSA::SceneContent::buildScenario()
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);

    // Define the scene
    Scenes::p0(*this);
    Scenes::p0a(*this);
    Scenes::p0b(*this);

    constexpr int      numPointClouds = 1;
    constexpr int      pointsPerCloud = 50;
    constexpr float    scaleFactor    = 1.0f;
    std::vector<Point> randomPointsFromCloud;
    std::vector<Point> extremumPointInCloud;

    // Practice 1:
    // Scenes::p1PointClouds(*this, numPointClouds, pointsPerCloud, scaleFactor, randomPointsFromCloud, extremumPointInCloud);
    // Scenes::p1Lines(*this, randomPointsFromCloud);
    // Scenes::p1Polygon(*this, extremumPointInCloud);
    // Scenes::p1Bezier(*this);
    // Scenes::p1Bezier(*this, true, 5);
    // Scenes::p1Intersections(*this);
    Scenes::p1All(*this);

    // Practice 2:
    Scenes::p2a(*this, numPointClouds, pointsPerCloud, scaleFactor);
    // Scenes::p2b(*this);
    // Scenes::p2c(*this);

    // Practice 3:
    // Scenes::p3(*this);

    // Practice 4:
    Scenes::p4a(*this, /*draw triangles*/ true);    // 2D Hull
    Scenes::p4b(*this, /*random on sphere surface*/ true);    // 3D Hull
    // Scenes::p4c(*this);    // 3D Hull from Trianles
    Scenes::p4d(*this);    // Voronoi Diagram
}

void GDSA::SceneContent::buildCamera(uint16_t width, uint16_t height)
{
    Camera* camera = new Camera(width, height, /*2D*/ false);
    camera->setPosition(glm::vec3(0.0f, 0.0f, 4.0f));
    camera->setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    camera->saveCamera();

    this->_camera.push_back(std::unique_ptr<Camera>(camera));
}

// ------------------------------------------------------------------------------------------

GDSA::SceneContent::~SceneContent()
{
    _camera.clear();
    _model.clear();
}

void GDSA::SceneContent::addNewCamera(ApplicationState* appState)
{
    _camera.push_back(std::make_unique<Camera>(appState->_viewportSize.x, appState->_viewportSize.y, /*2D*/ false));
}

void GDSA::SceneContent::addNewModel(Model3D* model)
{
    _model.push_back(std::unique_ptr<Model3D>(model));
}

GDSA::Model3D* GDSA::SceneContent::getModel(Model3D::Component* component) const
{
    for(auto& model : _model)
    {
        if(model->belongsModel(component))
            return model.get();
    }

    return nullptr;
}
