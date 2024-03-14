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

void AlgGeom::Scenes::p0(SceneContent& sc)
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);
    // Random segments
    constexpr int numSegments = 8;
    for(int segmentIdx = 0; segmentIdx < numSegments; ++segmentIdx)
    {
        Point        a(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        Point        b(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        SegmentLine* segment = new SegmentLine(a, b);

        sc.addNewModel((new DrawSegment(*segment))->setLineColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setLineWidth(RandomUtilities::getUniformRandom(1.0f, 3.0f)));
        delete segment;
    }

    // Random points
    constexpr int numPoints = 200;
    for(int pointIdx = 0; pointIdx < numPoints; ++pointIdx)
    {
        Point point(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x));
        sc.addNewModel((new DrawPoint(point))->setPointColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setPointSize(RandomUtilities::getUniformRandom(4.0f, 8.0f)));
    }

    // Polygon
    float           polygonAngle = .0f;
    constexpr float polygonAlpha = 2.0f * glm::pi<float>() / 5.0f;
    Polygon*        polygon      = new Polygon;

    while(polygonAngle < 2.0f * glm::pi<float>())
    {
        polygon->add(Point(std::cos(polygonAngle), std::sin(polygonAngle)));
        polygonAngle += polygonAlpha;
    }

    sc.addNewModel((new DrawPolygon(*polygon))->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName()->setModelMatrix(rotate(glm::mat4(1.0f), (glm::abs(4 * polygonAlpha - glm::pi<float>() / 2.0f * 3.0f)), glm::vec3(.0f, .0f, 1.0f))));
    delete polygon;
}

void AlgGeom::Scenes::p1PointClouds(SceneContent& sc, const int numPointClouds, int pointsPerCloud, float scaleFactor, std::vector<Point>& randomPointsFromCloud, std::vector<Point>& extremumPointInCloud)
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);

    // Tasks
    // Pr1-a-1: point cloud
    glm::vec3 center;
    for(int pcIdx = 0; pcIdx < numPointClouds; ++pcIdx)
    {
        auto* pointCloud = new PointCloud;
        // auto* pointCloud = new PointCloud("PointCloud" + std::to_string(pcIdx) + ".txt");

        float scale = scaleFactor;
        // if(numPointClouds > 1)
        if(pcIdx == 0)
        {
            scale /= 2.0f;
            center = glm::vec3(0, 0, 0);
        }
        else
        {
            scale *= 2.0f;
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

                // Disk point cloud
                // rand = RandomUtilities::getUniformRandomInUnitDiskCircumference() / scale + center;
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

        // sc.addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setPointSize(5.0f));
        sc.addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(RandomUtilities::getUniformRandomColorEuclideanDistance())->overrideModelName()->setPointSize(7.0f));
        // sc.addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setPointSize(RandomUtilities::getUniformRandom(4.0f, 8.0f)));
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
        // sc.addNewModel((new DrawBezier(*bezier))->setPointColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(8.0f)->setLineWidth(8.0f));
        // delete bezier;

        delete pointCloud;
    }
}

// blue segment, red line, and magenta ray
void AlgGeom::Scenes::p1Lines(SceneContent& sc, const std::vector<Point>& randomPointsFromCloud)
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);

    {
        // const Point  a(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Point  b(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Point  a(-2, 1);
        // const Point  b(2, 1);
        if(randomPointsFromCloud.size() >= 2)
        {
            auto* segment = new SegmentLine(randomPointsFromCloud.at(0), randomPointsFromCloud.at(1));
            sc.addNewModel((new DrawSegment(*segment))->setLineColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(5.0f)->setLineWidth(3.0f));
            sc.addNewModel((new DrawPoint(randomPointsFromCloud.at(0)))->setPointColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            sc.addNewModel((new DrawPoint(randomPointsFromCloud.at(1)))->setPointColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
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
            sc.addNewModel((new DrawLine(*line))->setLineColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(5.0f)->setLineWidth(1.0f));
            sc.addNewModel((new DrawPoint(randomPointsFromCloud.at(2)))->setPointColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            sc.addNewModel((new DrawPoint(randomPointsFromCloud.at(3)))->setPointColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
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
            auto* ray = new RayLine(randomPointsFromCloud.at(4), randomPointsFromCloud.at(5));
            sc.addNewModel((new DrawRay(*ray))->setLineColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(5.0f)->setLineWidth(2.0f));
            sc.addNewModel((new DrawPoint(randomPointsFromCloud.at(4)))->setPointColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            sc.addNewModel((new DrawPoint(randomPointsFromCloud.at(5)))->setPointColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            delete ray;
        }
    }
}

// Convex polygon from the point cloud extremum points
void AlgGeom::Scenes::p1Polygon(SceneContent& sc, const std::vector<Point>& extremumPointInCloud)
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);
    if(extremumPointInCloud.size() >= 4)
    {
        auto* polygon = new Polygon;
        for(auto& point : extremumPointInCloud)
        {
            polygon->add(point);
        }
        sc.addNewModel((new DrawPolygon(*polygon))->setLineColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName()->setPointSize(5.0f)->setLineWidth(2.0f));
        delete polygon;
    }
}
// Bezier curve
void AlgGeom::Scenes::p1Bezier(SceneContent& sc, bool randomPoints, const size_t pointNum)
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);
    std::vector<Vect2d> controlPoints;

    if(randomPoints)
    {
        for(size_t i = 0; i < pointNum; i++)
        {
            controlPoints.emplace_back(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        }
    }
    else
    {
        controlPoints.emplace_back(-2, -1);
        // controlPoints.emplace_back(-2, 1);
        controlPoints.emplace_back(-1, 1);
        // controlPoints.emplace_back(-1, 1);
        controlPoints.emplace_back(1, -1);
        // controlPoints.emplace_back(1, 1);
        controlPoints.emplace_back(2, 1);
        // controlPoints.emplace_back(2, -1);
    }

    for(size_t i = 0; i < controlPoints.size(); i++)
    {
        sc.addNewModel((new DrawPoint(controlPoints.at(i)))->setPointColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
        if(i > 0 && i < controlPoints.size())
        {
            auto* controlSegment = new SegmentLine(controlPoints.at(i - 1), controlPoints.at(i));
            sc.addNewModel((new DrawSegment(*controlSegment))->setLineColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(0.0f)->setLineWidth(1.0f));
            delete controlSegment;
        }
    }

    const auto* bezier = new Bezier(controlPoints, static_cast<int>(controlPoints.size()));
    sc.addNewModel((new DrawBezier(*bezier, 0.02f))->setPointColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(8.0f)->setLineWidth(8.0f));
    delete bezier;
}

// P1-B: Two segments, Two Lines, Two Rays, and a Polygon
void AlgGeom::Scenes::p1Intersections(SceneContent& sc)
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);

    SegmentLine* s1 = new SegmentLine({0, 2}, {1.5, 0.5});
    sc.addNewModel((new DrawSegment(*s1))->setLineColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    SegmentLine* s2 = new SegmentLine({-0.5, 1.7}, {0.5, 1.8});
    sc.addNewModel((new DrawSegment(*s2))->setLineColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    Vect2d s1_s2_intersection;
    if(s1->intersects(*s2, s1_s2_intersection))
    {
        std::cout << "Intersection between S1 and S2 at " << s1_s2_intersection << '\n';
        sc.addNewModel((new DrawPoint(s1_s2_intersection))->setPointColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(15.0f));
    }

    RayLine* r1 = new RayLine({0.8, 0.8}, {1.0, 2.5});
    sc.addNewModel((new DrawRay(*r1))->setLineColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    Vect2d s1_r1_intersection;
    if(s1->intersects(*r1, s1_r1_intersection))
    {
        std::cout << "Intersection between S1 and R1 at " << s1_r1_intersection << '\n';
        sc.addNewModel((new DrawPoint(s1_r1_intersection))->setPointColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(15.0f));
    }

    RayLine* r2 = new RayLine({-0.2, -0.2}, {-1.5, -1.0});
    sc.addNewModel((new DrawRay(*r2))->setLineColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    Line* l1 = new Line({0.0, 0.0}, {-1.5, 1.5});
    sc.addNewModel((new DrawLine(*l1))->setLineColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    Line* l2 = new Line({0.0, -2.0}, {1.5, -0.5});
    sc.addNewModel((new DrawLine(*l2))->setLineColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    Vect2d l1_l2_intersection;
    if(l1->intersects(*l2, l1_l2_intersection))
    {
        std::cout << "Intersection between L1 and L2 at " << l1_l2_intersection << '\n';
        sc.addNewModel((new DrawPoint(l1_l2_intersection))->setPointColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(15.0f));
    }

    // Polygon
    float           pAngle = .0f;
    constexpr float pAlpha = 2.0f * glm::pi<float>() / 7.0f;
    Polygon*        p      = new Polygon;
    while(pAngle < 2.0f * glm::pi<float>())
    {
        p->add(Point(std::cos(pAngle), std::sin(pAngle)));
        pAngle += pAlpha;
    }
    sc.addNewModel((new DrawPolygon(*p))->setLineColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    std::vector<Vect2d> p_r2_intersection;
    if(p->intersects(*r2, p_r2_intersection))
    {
        for(const auto& intersection : p_r2_intersection)
        {
            std::cout << "Intersection between R2 and Polygon at " << intersection << '\n';
            sc.addNewModel((new DrawPoint(intersection))->setPointColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(15.0f));
        }
    }

    std::vector<Vect2d> p_l1_intersection;
    if(p->intersects(*l1, p_l1_intersection))
    {
        for(const auto& intersection : p_l1_intersection)
        {
            std ::cout << "Intersection between L1 and Polygon at " << intersection << '\n';
            sc.addNewModel((new DrawPoint(intersection))->setPointColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(15.0f));
        }
    }

    std::cout << "Distances from the poligon points: \n";
    std::cout << "==================================================\n";
    for(auto& vertex : p->getVertices())
    {
        Vect2d intersection = vertex.getPoint();
        std::cout << "Distance from S1 to Polygon v#" << vertex.getPositionInPolygon() << " is " << s1->distPointSegment(intersection) << '\n';
        std::cout << "Distance from S2 to Polygon v#" << vertex.getPositionInPolygon() << " is " << s2->distPointSegment(intersection) << '\n';
        std::cout << "Distance from R1 to Polygon v#" << vertex.getPositionInPolygon() << " is " << r1->distPointSegment(intersection) << '\n';
        std::cout << "Distance from R2 to Polygon v#" << vertex.getPositionInPolygon() << " is " << r2->distPointSegment(intersection) << '\n';
        std::cout << "Distance from L1 to Polygon v#" << vertex.getPositionInPolygon() << " is " << l1->distPointSegment(intersection) << '\n';
        std::cout << "Distance from L2 to Polygon v#" << vertex.getPositionInPolygon() << " is " << l2->distPointSegment(intersection) << '\n';
    }
    std::cout << "==================================================\n";

    delete s1;
    delete s2;
    delete r1;
    delete r2;
    delete l1;
    delete l2;
    delete p;
}

void AlgGeom::Scenes::p1All(SceneContent& sc)
{
    constexpr int      numPointClouds = 3;
    constexpr int      pointsPerCloud = 100;
    constexpr float    scale          = 1.0f;
    std::vector<Point> randomPointsFromCloud;
    std::vector<Point> extremumPointInCloud;

    p1PointClouds(sc, numPointClouds, pointsPerCloud, scale, randomPointsFromCloud, extremumPointInCloud);
    p1Lines(sc, randomPointsFromCloud);
    p1Polygon(sc, extremumPointInCloud);
    p1Bezier(sc);
    p1Intersections(sc);
}
