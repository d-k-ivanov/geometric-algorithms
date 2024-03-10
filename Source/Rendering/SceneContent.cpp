#include "SceneContent.h"

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
    const glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    const glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);

    // Random segments
    int numSegments = 0;
    for(int segmentIdx = 0; segmentIdx < numSegments; ++segmentIdx)
    {
        Point        a(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        Point        b(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        SegmentLine* segment = new SegmentLine(a, b);

        this->addNewModel((new DrawSegment(*segment))->setLineColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setLineWidth(RandomUtilities::getUniformRandom(1.0f, 3.0f)));
        delete segment;
    }

    // Random points
    int numPoints = 0;
    for(int pointIdx = 0; pointIdx < numPoints; ++pointIdx)
    {
        Point point(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x));
        this->addNewModel((new DrawPoint(point))->setPointColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setPointSize(RandomUtilities::getUniformRandom(4.0f, 8.0f)));
    }

    // Polygon
    // float           polygonAngle = .0f;
    // constexpr float polygonAlpha = 2.0f * glm::pi<float>() / 5.0f;
    // Polygon*        polygon      = new Polygon;
    //
    // while(polygonAngle < 2.0f * glm::pi<float>())
    // {
    //     polygon->add(Point(std::cos(polygonAngle), std::sin(polygonAngle)));
    //     polygonAngle += polygonAlpha;
    // }
    //
    // this->addNewModel((new DrawPolygon(*polygon))->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName()->setModelMatrix(glm::rotate(glm::mat4(1.0f), (glm::abs(4 * polygonAlpha - glm::pi<float>() / 2.0f * 3.0f)), glm::vec3(.0f, .0f, 1.0f))));
    // delete polygon;

    // Tasks
    // Pr1-a-1: point cloud
    constexpr int      numPointClouds = 1;
    float              scale          = 1.0f;
    glm::vec3          center;
    std::vector<Point> randomPointsFromCloud;
    std::vector<Point> extremumPointInCloud;
    extremumPointInCloud.reserve(4);

    for(int pcIdx = 0; pcIdx < numPointClouds; ++pcIdx)
    {
        constexpr int pointsPerCloud = 50;
        PointCloud*   pointCloud     = new PointCloud;
        // PointCloud* pointCloud = new PointCloud("PointCloud" + std::to_string(pcIdx) + ".txt");

        // if(numPointClouds > 1)
        if(pcIdx == 0)
        {
            scale  = 0.5f;
            center = glm::vec3(0, 0, 0);
        }
        else
        {
            scale  = 3.0f;
            center = glm::vec3(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        }

        for(int idx = 0; idx < pointsPerCloud; ++idx)
        {
            glm::vec3 rand;

            if(pcIdx == 0)
            {
                // Hemispheric point cloud
                // rand = RandomUtilities::getUniformRandomInHemisphere(vec3(0, -1, 0)) / scale + center;

                // Spheric point cloud
                rand = RandomUtilities::getUniformRandomInUnitSphere() / scale + center;
            }
            else if(pcIdx % 2 == 0)
            {
                // Disk point cloud
                rand = RandomUtilities::getUniformRandomInUnitDiskCircumference() / scale + center;
                // rand = RandomUtilities::getUniformRandomCosineDirection() / scale + center;
                // rand = RandomUtilities::getUniformRandomInUnitDisk() / scale + center;
            }
            else
            {
                // Rectangular point cloud
                // rand = RandomUtilities::getUniformRandomInUnitSquare() / scale + center;
                rand = RandomUtilities::getUniformRandomInUnitSquarePerimeter() / scale + center;
            }
            pointCloud->addPoint(Point(rand.x, rand.y));
        }

        // this->addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setPointSize(5.0f));
        this->addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(RandomUtilities::getUniformRandomColorEuclideanDistance())->overrideModelName()->setPointSize(7.0f));
        // this->addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setPointSize(RandomUtilities::getUniformRandom(4.0f, 8.0f)));
        // pointCloud->save("PointCloud" + std::to_string(pcIdx) + ".txt");

        if(pcIdx == 0)
        {
            for(auto& point : pointCloud->getPoints())
            {
                std::cout << point << '\n';
                if(extremumPointInCloud.size() < 4)
                {
                    extremumPointInCloud.push_back(point);
                    extremumPointInCloud.push_back(point);
                    extremumPointInCloud.push_back(point);
                    extremumPointInCloud.push_back(point);
                }
                else
                {
                    if(point.getX() > extremumPointInCloud.at(0).getX())
                    {
                        extremumPointInCloud.at(0) = point;
                    }

                    if(point.getY() > extremumPointInCloud.at(1).getY())
                    {
                        extremumPointInCloud.at(1) = point;
                    }

                    if(point.getX() < extremumPointInCloud.at(2).getX())
                    {
                        extremumPointInCloud.at(2) = point;
                    }

                    if(point.getY() < extremumPointInCloud.at(3).getY())
                    {
                        extremumPointInCloud.at(3) = point;
                    }
                }
            }
            std::cout << "==================================================\n";
            std::cout << extremumPointInCloud.at(0) << '\n';
            std::cout << extremumPointInCloud.at(1) << '\n';
            std::cout << extremumPointInCloud.at(2) << '\n';
            std::cout << extremumPointInCloud.at(3) << '\n';
            std::cout << "==================================================\n";

            for(int i = 0; i < 7; ++i)
            {
                randomPointsFromCloud.push_back(pointCloud->getPoint(RandomUtilities::getUniformRandomInt(0, static_cast<int>(pointCloud->size()))));
            }
        }

        // Playing with bezier curve on the point cloud
        // Bezier* bezier = new Bezier(pointCloud->getPoints(), static_cast<int>(pointCloud->size()));
        // this->addNewModel((new DrawBezier(*bezier))->setPointColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(8.0f)->setLineWidth(8.0f));
        // delete bezier;

        delete pointCloud;
    }

    // blue segment, red line, and magenta ray
    {
        // const Point  a(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Point  b(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Point  a(-2, 1);
        // const Point  b(2, 1);
        if(randomPointsFromCloud.size() >= 2)
        {
            SegmentLine* segment = new SegmentLine(randomPointsFromCloud.at(0), randomPointsFromCloud.at(1));
            this->addNewModel((new DrawSegment(*segment))->setLineColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(5.0f)->setLineWidth(3.0f));
            this->addNewModel((new DrawPoint(randomPointsFromCloud.at(0)))->setPointColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            this->addNewModel((new DrawPoint(randomPointsFromCloud.at(1)))->setPointColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            delete segment;
        }
    }
    {
        // const Point a(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Point b(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Point a(-2, 0);
        // const Point b(2, 0);
        if(randomPointsFromCloud.size() >= 4)
        {
            Line* line = new Line(randomPointsFromCloud.at(2), randomPointsFromCloud.at(3));
            this->addNewModel((new DrawLine(*line))->setLineColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(5.0f)->setLineWidth(1.0f));
            this->addNewModel((new DrawPoint(randomPointsFromCloud.at(2)))->setPointColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            this->addNewModel((new DrawPoint(randomPointsFromCloud.at(3)))->setPointColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            delete line;
        }
    }
    {
        // const Point a(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Point b(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Point a(-2, -1);
        // const Point b(2, -1);
        if(randomPointsFromCloud.size() >= 6)
        {
            RayLine* ray = new RayLine(randomPointsFromCloud.at(4), randomPointsFromCloud.at(5));
            this->addNewModel((new DrawRay(*ray))->setLineColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(5.0f)->setLineWidth(2.0f));
            this->addNewModel((new DrawPoint(randomPointsFromCloud.at(4)))->setPointColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            this->addNewModel((new DrawPoint(randomPointsFromCloud.at(5)))->setPointColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            delete ray;
        }
    }
    // Convex polygon from the point cloud extremum points
    {
        Polygon* polygon = new Polygon;
        for(auto& point : extremumPointInCloud)
        {
            polygon->add(point);
        }
        this->addNewModel((new DrawPolygon(*polygon))->setLineColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName()->setPointSize(5.0f)->setLineWidth(2.0f));
        delete polygon;
    }
    // Bezier curve
    {
        std::vector<Vect2d> controlPoints;
        controlPoints.emplace_back(-2, -1);
        // controlPoints.emplace_back(-2, 1);
        // controlPoints.emplace_back(-1, 1);
        controlPoints.emplace_back(-1, 1);
        controlPoints.emplace_back(1, -1);
        // controlPoints.emplace_back(1, 1);
        // controlPoints.emplace_back(2, 1);
        controlPoints.emplace_back(2, 1);

        for(size_t i = 0; i < controlPoints.size(); i++)
        {
            this->addNewModel((new DrawPoint(controlPoints.at(i)))->setPointColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            if(i > 0 && i < controlPoints.size())
            {
                SegmentLine* controlSegment = new SegmentLine(controlPoints.at(i - 1), controlPoints.at(i));
                this->addNewModel((new DrawSegment(*controlSegment))->setLineColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(0.0f)->setLineWidth(1.0f));
                delete controlSegment;
            }
        }

        Bezier* bezier = new Bezier(controlPoints, static_cast<int>(controlPoints.size()));
        this->addNewModel((new DrawBezier(*bezier, 0.02f))->setPointColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(8.0f)->setLineWidth(8.0f));
        delete bezier;
    }
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
    _camera.push_back(std::unique_ptr<Camera>(new Camera(appState->_viewportSize.x, appState->_viewportSize.y, true)));
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
