#include "SceneContent.h"
#include "Scenes.h"

namespace GDSA::Render
{
// ----------------------------- BUILD YOUR SCENARIO HERE -----------------------------------

void SceneContent::buildScenario()
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);

    // Define the scene
    // Scenes::p0(*this);
    // Scenes::p0a(*this);
    // Scenes::p0b(*this);

    constexpr int                numPointClouds = 1;
    constexpr int                pointsPerCloud = 50;
    constexpr float              scaleFactor    = 1.0f;
    std::vector<Geometry::Point> randomPointsFromCloud;
    std::vector<Geometry::Point> extremumPointInCloud;

    // Practice 1:
    // Scenes::p1PointClouds(*this, numPointClouds, pointsPerCloud, scaleFactor, randomPointsFromCloud, extremumPointInCloud);
    // Scenes::p1Lines(*this, randomPointsFromCloud);
    // Scenes::p1Polygon(*this, extremumPointInCloud);
    // Scenes::p1Bezier(*this);
    // Scenes::p1Bezier(*this, /*use random point*/ true, /*number of random point*/ 5);
    // Scenes::p1Intersections(*this);
    // Scenes::p1All(*this);

    // Practice 2:
    // Scenes::p2a(*this, numPointClouds, pointsPerCloud, scaleFactor);
    // Scenes::p2b(*this);
    // Scenes::p2c(*this);

    // Practice 3:
    // Scenes::p3(*this);

    // Practice 4:
    // Scenes::p4a(*this, /*draw triangles*/ true);    // 2D Hull
    Scenes::p4b(*this, /*random on sphere surface*/ true);    // 3D Hull
    // Scenes::p4c(*this);    // 3D Hull from Trianles
    // Scenes::p4d(*this);    // Voronoi Diagram
}

void SceneContent::buildCamera(uint16_t width, uint16_t height)
{
    Camera* camera = new Camera(width, height, /*2D*/ false);
    camera->setPosition(glm::vec3(0.0f, 0.0f, 4.0f));
    camera->setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    camera->saveCamera();

    this->_camera.push_back(std::unique_ptr<Camera>(camera));
}

// ------------------------------------------------------------------------------------------

SceneContent::~SceneContent()
{
    _camera.clear();
    _model.clear();
}

void SceneContent::addNewCamera(ApplicationState* appState)
{
    _camera.push_back(std::make_unique<Camera>(appState->_viewportSize.x, appState->_viewportSize.y, /*2D*/ false));
}

void SceneContent::addNewModel(Model3D* model)
{
    _model.push_back(std::unique_ptr<Model3D>(model));
}

Model3D* SceneContent::getModel(Model3D::Component* component) const
{
    for(auto& model : _model)
    {
        if(model->belongsModel(component))
            return model.get();
    }

    return nullptr;
}
}    // namespace GDSA::Render
