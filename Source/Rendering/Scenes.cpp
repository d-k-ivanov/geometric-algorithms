// ReSharper disable CppTooWideScope

#include "Scenes.h"

#include "DrawAABB.h"
#include "DrawBezier.h"
#include "DrawCircle.h"
#include "DrawLine.h"
#include "DrawMesh.h"
#include "DrawPlane.h"
#include "DrawPoint.h"
#include "DrawPointCloud.h"
#include "DrawPolygon.h"
#include "DrawRay.h"
#include "DrawSegment.h"
#include "DrawTriangle.h"

#include "Geometry/Bezier.h"
#include "Geometry/Circle.h"
#include "Geometry/ConvexHull.h"
#include "Geometry/Plane.h"
#include "Geometry/PointCloud.h"
#include "Geometry/Polygon.h"
#include "Geometry/SegmentLine.h"
#include "Geometry/TriangleModel.h"
#include "Geometry/Triangulation.h"
#include "Geometry/Voxelization.h"

#include "Utils/ChronoUtilities.h"
#include "Utils/FilesystemUtilities.h"
#include "Utils/RandomUtilities.h"

#include <iostream>

namespace GDSA::Render
{
void Scenes::p0(SceneContent& sc)
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);
    // Random segments
    constexpr int numSegments = 8;
    for(int segmentIdx = 0; segmentIdx < numSegments; ++segmentIdx)
    {
        Geometry::Point        a(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x), Utils::Random::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        Geometry::Point        b(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x), Utils::Random::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        Geometry::SegmentLine* segment = new Geometry::SegmentLine(a, b);

        sc.addNewModel((new DrawSegment(*segment))->setLineColor(Utils::Random::getUniformRandomColor())->overrideModelName()->setLineWidth(Utils::Random::getUniformRandom(1.0f, 3.0f)));
        delete segment;
    }

    // Random points
    constexpr int numPoints = 200;
    for(int pointIdx = 0; pointIdx < numPoints; ++pointIdx)
    {
        Geometry::Point point(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x), Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x));
        sc.addNewModel((new DrawPoint(point))->setPointColor(Utils::Random::getUniformRandomColor())->overrideModelName()->setPointSize(Utils::Random::getUniformRandom(4.0f, 8.0f)));
    }

    // Polygon
    float              polygonAngle = .0f;
    constexpr float    polygonAlpha = 2.0f * glm::pi<float>() / 5.0f;
    Geometry::Polygon* polygon      = new Geometry::Polygon;

    while(polygonAngle < 2.0f * glm::pi<float>())
    {
        polygon->add(Geometry::Point(std::cos(polygonAngle), std::sin(polygonAngle)));
        polygonAngle += polygonAlpha;
    }

    sc.addNewModel((new DrawPolygon(*polygon))->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName()->setModelMatrix(rotate(glm::mat4(1.0f), (glm::abs(4 * polygonAlpha - glm::pi<float>() / 2.0f * 3.0f)), glm::vec3(.0f, .0f, 1.0f))));
    delete polygon;

    constexpr int   numTriangles = 30;
    constexpr float alpha        = 2 * glm::pi<float>() / static_cast<float>(numTriangles);

    for(int triangleIdx = 0; triangleIdx < numTriangles; ++triangleIdx)
    {
        const float         randB = Utils::Random::getUniformRandom(.5f, .9f);
        const float         randC = Utils::Random::getUniformRandom(.6f, .8f);
        Geometry::Vect2d    a(.0f, .0f);
        Geometry::Vect2d    b(glm::cos(alpha * static_cast<float>(triangleIdx)) * randB, glm::sin(alpha * static_cast<float>(triangleIdx)) * randB);
        Geometry::Vect2d    c(glm::cos(alpha * static_cast<float>(triangleIdx + 1)) * randC, glm::sin(alpha * static_cast<float>(triangleIdx + 1)) * randC);
        Geometry::Triangle* triangle = new Geometry::Triangle(a, b, c);

        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
}

void Scenes::p0a(SceneContent& sc)
{
    {
        Geometry::Vect3d      a {0.5f, -0.5f, 0.0f};
        Geometry::Vect3d      b {0.5f, -0.5f, 0.0f};
        Geometry::Vect3d      c {0.0f, 0.5f, 0.0f};
        Geometry::Triangle3d* triangle = new Geometry::Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Geometry::Vect3d      a {-0.5f, -0.5f, 0.0f};
        Geometry::Vect3d      b {0.0f, -0.5f, 0.5f};
        Geometry::Vect3d      c {0.0f, 0.5f, 0.0f};
        Geometry::Triangle3d* triangle = new Geometry::Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Geometry::Vect3d      a {-0.5f, -0.5f, 0.0f};
        Geometry::Vect3d      b {0.0f, -0.5f, -0.5f};
        Geometry::Vect3d      c {0.0f, 0.5f, 0.0f};
        Geometry::Triangle3d* triangle = new Geometry::Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Geometry::Vect3d      a {0.5f, -0.5f, 0.0f};
        Geometry::Vect3d      b {0.0f, -0.5f, 0.5f};
        Geometry::Vect3d      c {0.0f, 0.5f, 0.0f};
        Geometry::Triangle3d* triangle = new Geometry::Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Geometry::Vect3d      a {0.5f, -0.5f, 0.0f};
        Geometry::Vect3d      b {0.0f, -0.5f, -0.5f};
        Geometry::Vect3d      c {0.0f, 0.5f, 0.0f};
        Geometry::Triangle3d* triangle = new Geometry::Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Geometry::Vect3d      a {-0.5f, -0.5f, 0.0f};
        Geometry::Vect3d      b {0.5f, -0.5f, 0.0f};
        Geometry::Vect3d      c {0.0f, -0.5f, -0.5f};
        Geometry::Triangle3d* triangle = new Geometry::Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
}

void Scenes::p0b(SceneContent& sc)
{
    const auto  center     = Geometry::Point(0, 0);
    const auto* circle     = new Geometry::Circle(center, 2.0);
    const auto  drawCircle = new DrawCircle(*circle, 100, /*draw triangular segments*/ true);
    drawCircle->setPointColor(Utils::Random::getUniformRandomColor())->setPointSize(7.0f);
    drawCircle->setLineColor(Utils::Random::getUniformRandomColor())->setLineWidth(5.0);
    drawCircle->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f));
    drawCircle->overrideModelName();
    sc.addNewModel(drawCircle);
}

void Scenes::p1PointClouds(SceneContent& sc, const int numPointClouds, int pointsPerCloud, float scaleFactor, std::vector<Geometry::Point>& randomPointsFromCloud, std::vector<Geometry::Point>& extremumPointInCloud)
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);

    // Tasks
    // Pr1-a-1: point cloud
    glm::vec3 center;
    for(int pcIdx = 0; pcIdx < numPointClouds; ++pcIdx)
    {
        auto* pointCloud = new Geometry::PointCloud;
        // auto* pointCloud = new Geometry::PointCloud("PointCloud" + std::to_string(pcIdx) + ".txt");

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
            center = glm::vec3(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x), Utils::Random::getUniformRandom(minBoundaries.y, maxBoundaries.y), 0.0f);
        }

        for(int idx = 0; idx < pointsPerCloud; ++idx)
        {
            glm::vec3 rand;

            if(pcIdx == 0)
            {
                // Hemispheric point cloud
                // rand = Utils::Random::getUniformRandomInHemisphere(vec3(0, -1, 0)) / scale + center;

                // Spheric point cloud
                rand = Utils::Random::getUniformRandomInUnitSphere() / scale + center;

                // Disk point cloud
                // rand = Utils::Random::getUniformRandomInUnitDiskCircumference() / scale + center;
            }
            else if(pcIdx % 2 == 0)
            {
                // Disk point cloud
                rand = Utils::Random::getUniformRandomInUnitDiskCircumference() / scale + center;
                // rand = Utils::Random::getUniformRandomCosineDirection() / scale + center;
                // rand = Utils::Random::getUniformRandomInUnitDisk() / scale + center;
            }
            else
            {
                // Rectangular point cloud
                // rand = Utils::Random::getUniformRandomInUnitSquare() / scale + center;
                rand = Utils::Random::getUniformRandomInUnitSquarePerimeter() / scale + center;
            }
            pointCloud->addPoint(Geometry::Point(rand.x, rand.y));
        }

        // sc.addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(Utils::Random::getUniformRandomColor())->overrideModelName()->setPointSize(5.0f));
        sc.addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(Utils::Random::getUniformRandomColorEuclideanDistance())->overrideModelName()->setPointSize(7.0f));
        // sc.addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(Utils::Random::getUniformRandomColor())->overrideModelName()->setPointSize(Utils::Random::getUniformRandom(4.0f, 8.0f)));
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
                randomPointsFromCloud.push_back(pointCloud->getPoint(Utils::Random::getUniformRandomInt(0, static_cast<int>(pointCloud->size()))));
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
void Scenes::p1Lines(SceneContent& sc, const std::vector<Geometry::Point>& randomPointsFromCloud)
{
    // constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    // constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);

    {
        // const Geometry::Point  a(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x), Random::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Geometry::Point  b(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x), Random::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Geometry::Point  a(-2, 1);
        // const Geometry::Point  b(2, 1);
        if(randomPointsFromCloud.size() >= 2)
        {
            auto* segment = new Geometry::SegmentLine(randomPointsFromCloud.at(0), randomPointsFromCloud.at(1));
            sc.addNewModel((new DrawSegment(*segment))->setLineColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(5.0f)->setLineWidth(3.0f));
            sc.addNewModel((new DrawPoint(randomPointsFromCloud.at(0)))->setPointColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            sc.addNewModel((new DrawPoint(randomPointsFromCloud.at(1)))->setPointColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            delete segment;
        }
    }
    {
        // const Geometry::Point a(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x), Random::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Geometry::Point b(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x), Random::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Geometry::Point a(-2, 0);
        // const Geometry::Point b(2, 0);
        if(randomPointsFromCloud.size() >= 4)
        {
            Geometry::Line* line = new Geometry::Line(randomPointsFromCloud.at(2), randomPointsFromCloud.at(3));
            sc.addNewModel((new DrawLine(*line))->setLineColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(5.0f)->setLineWidth(1.0f));
            sc.addNewModel((new DrawPoint(randomPointsFromCloud.at(2)))->setPointColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            sc.addNewModel((new DrawPoint(randomPointsFromCloud.at(3)))->setPointColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            delete line;
        }
    }
    {
        // const Geometry::Point a(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x), Random::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Geometry::Point b(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x), Random::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        // const Geometry::Point a(-2, -1);
        // const Geometry::Point b(2, -1);
        if(randomPointsFromCloud.size() >= 6)
        {
            auto* ray = new Geometry::RayLine(randomPointsFromCloud.at(4), randomPointsFromCloud.at(5));
            sc.addNewModel((new DrawRay(*ray))->setLineColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(5.0f)->setLineWidth(2.0f));
            sc.addNewModel((new DrawPoint(randomPointsFromCloud.at(4)))->setPointColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            sc.addNewModel((new DrawPoint(randomPointsFromCloud.at(5)))->setPointColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(10.0f));
            delete ray;
        }
    }
}

// Convex polygon from the point cloud extremum points
void Scenes::p1Polygon(SceneContent& sc, const std::vector<Geometry::Point>& extremumPointInCloud)
{
    if(extremumPointInCloud.size() >= 4)
    {
        auto* polygon = new Geometry::Polygon;
        for(auto& point : extremumPointInCloud)
        {
            polygon->add(point);
        }
        sc.addNewModel((new DrawPolygon(*polygon))->setLineColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName()->setPointSize(5.0f)->setLineWidth(2.0f));
        delete polygon;
    }
}
// Bezier curve
void Scenes::p1Bezier(SceneContent& sc, bool randomPoints, const size_t pointNum)
{
    constexpr glm::vec2           minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2           maxBoundaries = glm::vec2(-minBoundaries);
    std::vector<Geometry::Vect2d> controlPoints;

    if(randomPoints)
    {
        for(size_t i = 0; i < pointNum; i++)
        {
            controlPoints.emplace_back(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x), Utils::Random::getUniformRandom(minBoundaries.y, maxBoundaries.y));
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
            auto* controlSegment = new Geometry::SegmentLine(controlPoints.at(i - 1), controlPoints.at(i));
            sc.addNewModel((new DrawSegment(*controlSegment))->setLineColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(0.0f)->setLineWidth(1.0f));
            delete controlSegment;
        }
    }

    const auto* bezier = new Geometry::Bezier(controlPoints, static_cast<int>(controlPoints.size()));
    sc.addNewModel((new DrawBezier(*bezier, 0.02f))->setPointColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(8.0f)->setLineWidth(8.0f));
    delete bezier;
}

// P1-B: Two segments, Two Lines, Two Rays, and a Polygon
void Scenes::p1Intersections(SceneContent& sc)
{
    auto* s1 = new Geometry::SegmentLine({0, 2}, {1.5, 0.5});
    sc.addNewModel((new DrawSegment(*s1))->setLineColor(Utils::Random::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    auto* s2 = new Geometry::SegmentLine({-0.5, 1.7}, {0.5, 1.8});
    sc.addNewModel((new DrawSegment(*s2))->setLineColor(Utils::Random::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    Geometry::Vect2d s1_s2_intersection;
    if(s1->intersects(*s2, s1_s2_intersection))
    {
        std::cout << "Intersection between S1 and S2 at " << s1_s2_intersection << '\n';
        sc.addNewModel((new DrawPoint(s1_s2_intersection))->setPointColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(15.0f));
    }

    auto* r1 = new Geometry::RayLine({0.8, 0.8}, {1.0, 2.5});
    sc.addNewModel((new DrawRay(*r1))->setLineColor(Utils::Random::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    Geometry::Vect2d s1_r1_intersection;
    if(s1->intersects(*r1, s1_r1_intersection))
    {
        std::cout << "Intersection between S1 and R1 at " << s1_r1_intersection << '\n';
        sc.addNewModel((new DrawPoint(s1_r1_intersection))->setPointColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(15.0f));
    }

    auto* r2 = new Geometry::RayLine({-0.2, -0.2}, {-1.5, -1.0});
    sc.addNewModel((new DrawRay(*r2))->setLineColor(Utils::Random::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    auto* l1 = new Geometry::Line({0.0, 0.0}, {-1.5, 1.5});
    sc.addNewModel((new DrawLine(*l1))->setLineColor(Utils::Random::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    auto* l2 = new Geometry::Line({0.0, -2.0}, {1.5, -0.5});
    sc.addNewModel((new DrawLine(*l2))->setLineColor(Utils::Random::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    Geometry::Vect2d l1_l2_intersection;
    if(l1->intersects(*l2, l1_l2_intersection))
    {
        std::cout << "Intersection between L1 and L2 at " << l1_l2_intersection << '\n';
        sc.addNewModel((new DrawPoint(l1_l2_intersection))->setPointColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(15.0f));
    }

    // Polygon
    float           pAngle = .0f;
    constexpr float pAlpha = 2.0f * glm::pi<float>() / 7.0f;
    auto*           p      = new Geometry::Polygon;
    while(pAngle < 2.0f * glm::pi<float>())
    {
        p->add(Geometry::Point(std::cos(pAngle), std::sin(pAngle)));
        pAngle += pAlpha;
    }
    sc.addNewModel((new DrawPolygon(*p))->setLineColor(Utils::Random::getUniformRandomColor())->overrideModelName()->setLineWidth(3.0f));

    std::vector<Geometry::Vect2d> p_r2_intersection;
    if(p->intersects(*r2, p_r2_intersection))
    {
        for(const auto& intersection : p_r2_intersection)
        {
            std::cout << "Intersection between R2 and Polygon at " << intersection << '\n';
            sc.addNewModel((new DrawPoint(intersection))->setPointColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setPointSize(15.0f));
        }
    }

    std::vector<Geometry::Vect2d> p_l1_intersection;
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
        Geometry::Vect2d intersection = vertex.getPoint();
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

void Scenes::p1All(SceneContent& sc)
{
    constexpr int                numPointClouds = 3;
    constexpr int                pointsPerCloud = 100;
    constexpr float              scale          = 1.0f;
    std::vector<Geometry::Point> randomPointsFromCloud;
    std::vector<Geometry::Point> extremumPointInCloud;

    p1PointClouds(sc, numPointClouds, pointsPerCloud, scale, randomPointsFromCloud, extremumPointInCloud);
    p1Lines(sc, randomPointsFromCloud);
    p1Polygon(sc, extremumPointInCloud);
    p1Bezier(sc);
    p1Intersections(sc);
}

void Scenes::p2a(SceneContent& sc, int numPointClouds, int pointsPerCloud, float scaleFactor)
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);

    // Tasks
    // Pr2-a-1: point cloud
    glm::vec3 center;
    for(int pcIdx = 0; pcIdx < numPointClouds; ++pcIdx)
    {
        auto* pointCloud = new Geometry::PointCloud3d;

        float scale = scaleFactor;
        if(pcIdx == 0)
        {
            scale /= 2.0f;
            center = glm::vec3(0, 0, 0);
        }
        else
        {
            scale *= 2.0f;
            center = glm::vec3(
                Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x),
                Utils::Random::getUniformRandom(minBoundaries.y, maxBoundaries.y),
                Utils::Random::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        }

        for(int idx = 0; idx < pointsPerCloud; ++idx)
        {
            glm::vec3 rand;

            if(pcIdx == 0)
            {
                // Hemispheric point cloud
                // rand = Utils::Random::getUniformRandomInHemisphere(vec3(0, -1, 0)) / scale + center;

                // Spheric point cloud
                rand = Utils::Random::getUniformRandomInUnitSphere() / scale + center;

                // Disk point cloud
                // rand = Utils::Random::getUniformRandomInUnitDiskCircumference() / scale + center;
            }
            else if(pcIdx % 2 == 0)
            {
                // Disk point cloud
                rand = Utils::Random::getUniformRandomInUnitDiskCircumference() / scale + center;
                // rand = Utils::Random::getUniformRandomCosineDirection() / scale + center;
                // rand = Utils::Random::getUniformRandomInUnitDisk() / scale + center;
            }
            else
            {
                // Rectangular point cloud
                // rand = Utils::Random::getUniformRandomInUnitSquare() / scale + center;
                rand = Utils::Random::getUniformRandomInUnitSquarePerimeter() / scale + center;
            }
            pointCloud->addPoint({rand.x, rand.y, rand.z});
        }

        // sc.addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(Utils::Random::getUniformRandomColor())->overrideModelName()->setPointSize(5.0f));
        sc.addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(Utils::Random::getUniformRandomColorEuclideanDistance())->overrideModelName()->setPointSize(7.0f));

        if(pcIdx == 0 and pointCloud->size() >= 2)
        {
            // Line L1
            auto l1A = pointCloud->getPoint(Utils::Random::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));
            auto l1B = pointCloud->getPoint(Utils::Random::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));

            // Test parallel:
            // Geometry::Vect3d l1A(-2, 0, -1);
            // Geometry::Vect3d l1B(2, 0, -1);

            // Test perpendicular:
            // Geometry::Vect3d l1A(0, -2, -1);
            // Geometry::Vect3d l1B(0, 2, -1);
            auto* l1       = new Geometry::Line3d(l1A, l1B);
            auto  l1Colour = Utils::Random::getUniformRandomColor();
            sc.addNewModel((new DrawLine(*l1))->setLineColor(l1Colour)->overrideModelName()->setPointSize(5.0f)->setLineWidth(3.0f));
            sc.addNewModel((new DrawPoint(l1A))->setPointColor(l1Colour)->overrideModelName()->setPointSize(30.0f));
            sc.addNewModel((new DrawPoint(l1B))->setPointColor(l1Colour)->overrideModelName()->setPointSize(30.0f));

            // Line L2
            auto l2A = pointCloud->getPoint(Utils::Random::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));
            auto l2B = pointCloud->getPoint(Utils::Random::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));

            // Replace to test if lines are parallel or perpendicular.
            // Geometry::Vect3d l2A(-2, 1, -1);
            // Geometry::Vect3d l2B(2, 1, -1);
            auto* l2       = new Geometry::Line3d(l2A, l2B);
            auto  l2Colour = Utils::Random::getUniformRandomColor();
            sc.addNewModel((new DrawLine(*l2))->setLineColor(l2Colour)->overrideModelName()->setPointSize(5.0f)->setLineWidth(3.0f));
            sc.addNewModel((new DrawPoint(l2A))->setPointColor(l2Colour)->overrideModelName()->setPointSize(10.0f));
            sc.addNewModel((new DrawPoint(l2B))->setPointColor(l2Colour)->overrideModelName()->setPointSize(10.0f));

            // Ray R
            auto rA = pointCloud->getPoint(Utils::Random::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));
            auto rB = pointCloud->getPoint(Utils::Random::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));
            // Vect3d rA(-2, 2, -1);
            // Vect3d rB(2, 2, -1);
            auto* r       = new Geometry::Ray3d(rA, rB);
            auto  rColour = Utils::Random::getUniformRandomColor();
            sc.addNewModel((new DrawRay(*r))->setLineColor(rColour)->overrideModelName()->setPointSize(5.0f)->setLineWidth(3.0f));
            sc.addNewModel((new DrawPoint(rA))->setPointColor(rColour)->overrideModelName()->setPointSize(10.0f));
            sc.addNewModel((new DrawPoint(rB))->setPointColor(rColour)->overrideModelName()->setPointSize(10.0f));

            // Segment S
            auto sA = pointCloud->getPoint(Utils::Random::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));
            auto sB = pointCloud->getPoint(Utils::Random::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));
            // Vect3d sA(-2,-1, -1);
            // Vect3d sB(2, -1, -1);
            auto* s       = new Geometry::Segment3d(sA, sB);
            auto  sColour = Utils::Random::getUniformRandomColor();
            sc.addNewModel((new DrawSegment(*s))->setLineColor(sColour)->overrideModelName()->setPointSize(5.0f)->setLineWidth(3.0f));
            sc.addNewModel((new DrawPoint(s->getOrigin()))->setPointColor(sColour)->overrideModelName()->setPointSize(10.0f));
            sc.addNewModel((new DrawPoint(s->getDestination()))->setPointColor(sColour)->overrideModelName()->setPointSize(10.0f));

            std::cout << "==================================================\n";

            if(l1->isPerpendicular(*l2))
            {
                std::cout << "L1 and L2 are perpendicular\n";
            }
            else
            {
                std::cout << "L1 and L2 are not perpendicular\n";
            }

            if(l1->isParallel(*l2))
            {
                std::cout << "L1 and L2 are parallel\n";
            }
            else
            {
                std::cout << "L1 and L2 are not parallel\n";
            }

            std::cout << "==================================================\n";

            double           maxDistance = 0;
            Geometry::Vect3d theMostDistantPoint;
            auto             sLine = new Geometry::Line3d(s->getOrigin(), s->getDestination());
            for(auto& point : pointCloud->getPoints())
            {
                double distance = sLine->distance(point);
                if(distance > maxDistance)
                {
                    maxDistance         = distance;
                    theMostDistantPoint = point;
                }
            }
            sc.addNewModel((new DrawPoint(theMostDistantPoint))->setPointColor(Utils::Random::getUniformRandomColor())->overrideModelName()->setPointSize(20.0f));

            // Line L3
            auto* l3       = new Geometry::Line3d(l1->normalLine(theMostDistantPoint));
            auto  l3Colour = Utils::Random::getUniformRandomColor();
            sc.addNewModel((new DrawLine(*l3))->setLineColor(l3Colour)->overrideModelName()->setPointSize(5.0f)->setLineWidth(3.0f));
            sc.addNewModel((new DrawPoint(l3->getOrigin()))->setPointColor(l3Colour)->overrideModelName()->setPointSize(10.0f));
            sc.addNewModel((new DrawPoint(l3->getDestination()))->setPointColor(l3Colour)->overrideModelName()->setPointSize(10.0f));

            if(l1->isPerpendicular(*l3))
            {
                std::cout << "L1 and L3 are perpendicular\n";
            }
            else
            {
                std::cout << "L1 and L3 are not perpendicular\n";
            }
            std::cout << "==================================================\n";
            std::cout << "Distance between L1 and L2 is " << l1->distance(*l2) << '\n';
            std::cout << "==================================================\n";

            auto* pointCloudAABB = new Geometry::AABB(pointCloud->getAABB());
            sc.addNewModel((new DrawAABB(*pointCloudAABB))->overrideModelName());

            Geometry::Vect3d min1(pointCloudAABB->getMin());
            Geometry::Vect3d min2(pointCloudAABB->getMin().getX(), pointCloudAABB->getMin().getY(), pointCloudAABB->getMax().getZ());
            Geometry::Vect3d min3(pointCloudAABB->getMax().getX(), pointCloudAABB->getMin().getY(), pointCloudAABB->getMin().getZ());
            auto*            lowerPlane = new Geometry::Plane(min1, min2, min3, true);
            sc.addNewModel((new DrawPlane(*lowerPlane))->overrideModelName()->setLineWidth(5.0)->setLineColor(glm::vec3(1, 1, 1)));

            delete l1;
            delete l2;
            delete l3;
            delete s;
            delete r;
            delete sLine;
            delete pointCloudAABB;
            delete lowerPlane;
        }

        delete pointCloud;
    }
}

void Scenes::p2b(SceneContent& sc)
{
    constexpr glm::vec3 minBoundaries = glm::vec3(-3.5, -1.5, -2.5);
    constexpr glm::vec3 maxBoundaries = glm::vec3(-minBoundaries);

    const Geometry::Vect3d pRand(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x),
                                 Utils::Random::getUniformRandom(minBoundaries.y, maxBoundaries.y),
                                 Utils::Random::getUniformRandom(minBoundaries.z, maxBoundaries.z));

    const Geometry::Vect3d uRand(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x),
                                 Utils::Random::getUniformRandom(minBoundaries.y, maxBoundaries.y),
                                 Utils::Random::getUniformRandom(minBoundaries.z, maxBoundaries.z));

    const Geometry::Vect3d vRand(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x),
                                 Utils::Random::getUniformRandom(minBoundaries.y, maxBoundaries.y),
                                 Utils::Random::getUniformRandom(minBoundaries.z, maxBoundaries.z));

    // const Geometry::Vect3d pRand(0, 0, 0);
    // const Geometry::Vect3d uRand(1, 1, 1);
    // const Geometry::Vect3d vRand(-1, 1, -1);

    // const auto randomColour = Utils::Random::getUniformRandomColor();
    // sc.addNewModel((new DrawPoint(pRand))->setPointColor(randomColour)->overrideModelName()->setPointSize(10.0f));
    // sc.addNewModel((new DrawPoint(uRand))->setPointColor(randomColour)->overrideModelName()->setPointSize(10.0f));
    // sc.addNewModel((new DrawPoint(vRand))->setPointColor(randomColour)->overrideModelName()->setPointSize(10.0f));

    auto* randomPlane = new Geometry::Plane(pRand, uRand, vRand, true);
    sc.addNewModel((new DrawPlane(*randomPlane))->overrideModelName()->setLineWidth(3.0)->setLineColor(glm::vec3(1, 1, 1)));

    const Geometry::Vect3d vRand2(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x),
                                  Utils::Random::getUniformRandom(minBoundaries.y, maxBoundaries.y),
                                  Utils::Random::getUniformRandom(minBoundaries.z, maxBoundaries.z));

    Geometry::Vect3d newPlaneNormal;
    double           newPlaneD;
    pRand.getPlane(uRand, newPlaneNormal, newPlaneD);
    Geometry::Vect3d newPlaneP1(0, 0, -newPlaneD / newPlaneNormal.getZ());
    Geometry::Vect3d newPlaneP2(-newPlaneD / newPlaneNormal.getX(), 0, 0);
    Geometry::Vect3d newPlaneP3(0, -newPlaneD / newPlaneNormal.getY(), 0);
    auto*            newPlane = new Geometry::Plane(newPlaneP1, newPlaneP2, newPlaneP3, true);
    // sc.addNewModel((new DrawPlane(*newPlane))->overrideModelName()->setLineWidth(3.0)->setLineColor(glm::vec3(0, 1, 1)));

    Geometry::Line3d intersectionLine;
    if(randomPlane->intersect(*newPlane, intersectionLine))
    {
        sc.addNewModel((new DrawLine(intersectionLine))->overrideModelName()->setLineWidth(5.0)->setLineColor(glm::vec3(1, 1, 0)));
    }
    else
    {
        std::cout << "Planes don't intersect\n";
    }

    Geometry::Vect3d v1(Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x),
                        Utils::Random::getUniformRandom(minBoundaries.y, maxBoundaries.y),
                        Utils::Random::getUniformRandom(minBoundaries.z, maxBoundaries.z));

    sc.addNewModel((new DrawPoint(v1))->setPointColor(glm::vec3(0, 0, 1))->overrideModelName()->setPointSize(20.0));
    std::cout << "The distance from V1 to randomPlane: " << randomPlane->distance(v1) << '\n';

    Geometry::Vect3d v2(randomPlane->reflectedPoint(v1));
    sc.addNewModel((new DrawPoint(v2))->setPointColor(glm::vec3(0, 1, 1))->overrideModelName()->setPointSize(20.0));
    std::cout << "The distance from V2(reflected) to randomPlane: " << randomPlane->distance(v2) << '\n';

    delete randomPlane;
    delete newPlane;
}

void Scenes::p2c(SceneContent& sc)
{
    auto*      triangleModel = new Geometry::TriangleModel(Utils::ThisExecutableLocation() + "/Resources/Models/Ajax.obj");
    const auto model(new DrawMesh(*triangleModel));
    model->setModelMatrix(translate(model->getModelMatrix(), glm::vec3(0.0f, 0.0f, 0.0f)));
    model->setModelMatrix(rotate(model->getModelMatrix(), -0.2f, glm::vec3(0.0f, 1.0f, 0.0f)))->overrideModelName();
    sc.addNewModel(model);

    {
        Geometry::Vect3d a {0.0, 0.0, 0.0};
        Geometry::Vect3d b {2.0, 0.0, 0.0};
        Geometry::Vect3d c {1.0, 1.0, 0.0};
        auto*            triangle = new Geometry::Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Geometry::Vect3d a {0.0, 0.0, 0.0};
        Geometry::Vect3d b {0.0, 2.0, 0.0};
        Geometry::Vect3d c {-1.0, 1.0, 0.0};
        auto*            triangle = new Geometry::Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Geometry::Vect3d a {0.0, 0.0, 0.0};
        Geometry::Vect3d b {-2.0, 0.0, 0.0};
        Geometry::Vect3d c {-1.0, -1.0, 0.0};
        auto*            triangle = new Geometry::Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Geometry::Vect3d a {0.0, 0.0, 0.0};
        Geometry::Vect3d b {0.0, -2.0, 0.0};
        Geometry::Vect3d c {1.0, -1.0, 0.0};
        auto*            triangle = new Geometry::Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Geometry::Vect3d a {0.0, 0.0, 0.0};
        Geometry::Vect3d b {0.0, 0.0, 2.0};
        Geometry::Vect3d c {1.0, 0.0, 1.0};
        auto*            triangle = new Geometry::Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Geometry::Vect3d a {0.0, 0.0, 0.0};
        Geometry::Vect3d b {0.0, 0.0, -2.0};
        Geometry::Vect3d c {1.0, 0.0, -1.0};
        auto*            triangle = new Geometry::Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
}

void Scenes::p3(SceneContent& sc)
{
    auto*      triangleModel1 = new Geometry::TriangleModel(Utils::ThisExecutableLocation() + "/Resources/Models/Ajax.obj");
    const auto model1(new DrawMesh(*triangleModel1));
    model1->setModelMatrix(translate(model1->getModelMatrix(), glm::vec3(0.0f, 0.0f, 0.0f)));
    model1->setModelMatrix(rotate(model1->getModelMatrix(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)))->overrideModelName();
    // sc.addNewModel(model1);

    // auto* triangleModel2 = new Geometry::TriangleModel(Utils::ThisExecutableLocation() + "/Resources/Models/Cheburashka.obj");
    // const auto     model2(new DrawMesh(*triangleModel2));
    // model2->setModelMatrix(glm::translate(model2->getModelMatrix(), glm::vec3(0.0f, 0.0f, 0.0f)));
    // model2->setModelMatrix(glm::rotate(model2->getModelMatrix(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)))->overrideModelName();
    // sc.addNewModel(model2);

    const glm::vec3& voxelSize = glm::vec3(0.05f);

    // Utils::Time::initChrono();
    // std::cout << "Voxelization (Brute Force) started\n";
    // Voxelization* voxelizationBruteForce = new Voxelization(triangleModel1, voxelSize, 0);
    // std::cout << "Voxelization (Brute Force) ended. Duration: " << Utils::Time::getDuration() << '\n';

    Utils::Time::initChrono();
    std::cout << "Voxelization (Line Sweep) started\n";
    auto* voxelizationLineSweep = new Geometry::Voxelization(triangleModel1, voxelSize, 1);
    std::cout << "Voxelization (Line Sweep) ended. Duration: " << Utils::Time::getDuration() << '\n';

    // Utils::Time::initChrono();
    // std::cout << "Voxelization (AABB) started\n";
    // auto* voxelizationAABB = new Geometry::Voxelization(triangleModel1, voxelSize, 2);
    // std::cout << "Voxelization (AABB) ended. Duration: " << Utils::Time::getDuration() << '\n';

    // const auto voxModelBruteForce(voxelizationBruteForce->getRenderingObject(/*outline mode*/ false));
    // voxModelBruteForce->setModelMatrix(glm::translate(model1->getModelMatrix(), glm::vec3(0.0f, 0.0f, 0.0f)));
    // voxModelBruteForce->setModelMatrix(glm::rotate(model1->getModelMatrix(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)))->overrideModelName();
    // sc.addNewModel(voxModelBruteForce);

    const auto voxModelLineSweep(voxelizationLineSweep->getRenderingObject(/*outline mode*/ false));
    voxModelLineSweep->setModelMatrix(translate(model1->getModelMatrix(), glm::vec3(0.0f, 0.0f, 0.0f)));
    voxModelLineSweep->setModelMatrix(rotate(model1->getModelMatrix(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)))->overrideModelName();
    sc.addNewModel(voxModelLineSweep);

    // const auto voxModelAABB(voxelizationAABB->getRenderingObject(/*outline mode*/ false));
    // voxModelAABB->setModelMatrix(glm::translate(model->getModelMatrix(), glm::vec3(0.0f, 0.0f, 0.0f)));
    // voxModelAABB->setModelMatrix(glm::rotate(model1->getModelMatrix(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)))->overrideModelName();
    // sc.addNewModel(voxModelAABB);
}

// Practice 4a: 2D Convex Hull
void Scenes::p4a(SceneContent& sc, bool drawTriangles)
{
    constexpr int       pointsPerCloud = 100;
    constexpr float     scale          = 0.5f;
    constexpr glm::vec3 center         = glm::vec3(0, 0, 0);

    auto* pointCloud2D = new Geometry::PointCloud;
    for(int idx = 0; idx < pointsPerCloud; ++idx)
    {
        const glm::vec3 rand = Utils::Random::getUniformRandomInUnitSphere() / scale + center;
        pointCloud2D->addPoint(Geometry::Point(rand.x, rand.y));
    }
    sc.addNewModel((new DrawPointCloud(*pointCloud2D))->setPointColor(Utils::Random::getUniformRandomColorEuclideanDistance())->overrideModelName()->setPointSize(10.0f));

    const Geometry::ConvexHull convexHull2D(pointCloud2D);
    const auto                 randColor1 = Utils::Random::getUniformRandomColorEuclideanDistance();
    for(size_t i = 0; i < convexHull2D.getPoints2D().size(); i++)
    {
        auto* segment = new Geometry::SegmentLine(convexHull2D.getPoints2D()[i], convexHull2D.getPoints2D()[(i + 1) % convexHull2D.getPoints2D().size()]);
        sc.addNewModel((new DrawSegment(*segment))->setLineColor(randColor1)->overrideModelName()->setLineWidth(5.0f));
        sc.addNewModel((new DrawPoint(segment->getA()))->setPointColor(randColor1)->overrideModelName()->setPointSize(15.0f));
        delete segment;
    }

    if(drawTriangles)
    {
        for(auto triangle : convexHull2D.getTriangles2D())
        {
            sc.addNewModel((new DrawTriangle(triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
        }
    }
}

// Practice 4b: 3D Convex Hull
void Scenes::p4b(SceneContent& sc, bool randomOnSphereSurface)
{
    constexpr int       pointsPerCloud = 100;
    constexpr float     scale          = 0.5f;
    constexpr glm::vec3 center         = glm::vec3(0, 0, 0);

    auto* pointCloud3D = new Geometry::PointCloud3d;

    for(int idx = 0; idx < pointsPerCloud; ++idx)
    {
        glm::vec3 rand;
        if(randomOnSphereSurface)
        {
            rand = Utils::Random::getUniformRandomInUnitSphereSurface() / scale + center;
        }
        else
        {
            rand = Utils::Random::getUniformRandomInUnitSphere() / scale + center;
        }
        pointCloud3D->addPoint({rand.x, rand.y, rand.z});
    }
    // sc.addNewModel((new DrawPointCloud(*pointCloud3D))->setPointColor(Utils::Random::getUniformRandomColorEuclideanDistance())->overrideModelName()->setPointSize(10.0f));

    const Geometry::ConvexHull convexHull3D(pointCloud3D);
    // auto color = Utils::Random::getUniformRandomColorEuclideanDistance();
    // for(size_t i = 0; i < convexHull3D.getPoints3D().size(); i++)
    // {
    //     auto orig = convexHull3D.getPoints3D()[i];
    //     auto dest = convexHull3D.getPoints3D()[(i + 1) % convexHull3D.getPoints3D().size()];
    //
    //     auto* segment3D = new Geometry::Segment3d(orig, dest);
    //     sc.addNewModel((new DrawSegment(*segment3D))->setLineColor(color)->overrideModelName()->setLineWidth(5.0f));
    //     sc.addNewModel((new DrawPoint(segment3D->getOrigin()))->setPointColor(color)->overrideModelName()->setPointSize(15.0f));
    //     delete segment3D;
    // }

    // for(auto s : convexHull3D.getSegments())
    // {
    //     sc.addNewModel((new DrawSegment(s))->setLineColor(color)->overrideModelName()->setLineWidth(5.0f));
    // }

    for(auto triangle : convexHull3D.getTriangles3D())
    {
        sc.addNewModel((new DrawTriangle(triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
    }
}

void Scenes::p4c(SceneContent& sc)
{
    auto*      triangleModel = new Geometry::TriangleModel(Utils::ThisExecutableLocation() + "/Resources/Models/Ajax.obj");
    const auto model(new DrawMesh(*triangleModel));
    model->setModelMatrix(translate(model->getModelMatrix(), glm::vec3(0.0f, 0.0f, 0.0f)));
    model->setModelMatrix(rotate(model->getModelMatrix(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)))->overrideModelName();
    sc.addNewModel(model);

    // auto cloud = triangleModel->getCloud();
    // ConvexHull convexHull3D(&cloud);
    const Geometry::ConvexHull convexHull3D(triangleModel);

    // const auto color = Utils::Random::getUniformRandomColorEuclideanDistance();
    // for(size_t i = 0; i < convexHull3D.getPoints3D().size(); i++)
    // {
    //     auto orig = convexHull3D.getPoints3D()[i];
    //     auto dest = convexHull3D.getPoints3D()[(i + 1) % convexHull3D.getPoints3D().size()];
    //
    //     auto* segment3D = new Geometry::Segment3d(orig, dest);
    //     sc.addNewModel((new DrawSegment(*segment3D))->setLineColor(color)->overrideModelName()->setLineWidth(5.0f));
    //     sc.addNewModel((new DrawPoint(segment3D->getOrigin()))->setPointColor(color)->overrideModelName()->setPointSize(15.0f));
    //     delete segment3D;
    // }

    // for(auto s : convexHull3D.getSegments())
    // {
    //     sc.addNewModel((new DrawSegment(s))->setLineColor(color)->overrideModelName()->setLineWidth(5.0f));
    // }

    for(auto triangle : convexHull3D.getTriangles3D())
    {
        sc.addNewModel((new DrawTriangle(triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
    }
}

void Scenes::p4d(SceneContent& sc)
{
    constexpr int       pointsPerCloud = 100;
    constexpr float     scale          = 0.5f;
    constexpr glm::vec3 center         = glm::vec3(0, 0, 0);

    auto* pointCloud2D = new Geometry::PointCloud;
    for(int idx = 0; idx < pointsPerCloud; ++idx)
    {
        const glm::vec3 rand = Utils::Random::getUniformRandomInUnitSphere() / scale + center;
        pointCloud2D->addPoint(Geometry::Point(rand.x, rand.y));
    }
    sc.addNewModel((new DrawPointCloud(*pointCloud2D))->setPointColor(Utils::Random::getUniformRandomColorEuclideanDistance())->overrideModelName()->setPointSize(5.0f));

    const Geometry::Triangulation triangulation2D(pointCloud2D);
    for(const auto& [line, isSegment] : triangulation2D.getEdges())
    {
        const auto color = Utils::Random::getUniformRandomColor();
        if(isSegment)
        {
            sc.addNewModel((new DrawSegment(*line))->overrideModelName()->setLineColor(color)->setLineWidth(3.0f)->setPointColor(color)->setPointSize(5.0f));
        }
        else
        {
            Geometry::RayLine ray(line->getA(), line->getB());
            sc.addNewModel((new DrawRay(ray))->overrideModelName()->setLineColor(color)->setLineWidth(3.0f)->setPointColor(color)->setPointSize(5.0f));
        }
    }
}

void Scenes::p10RandomPoints(SceneContent& sc)
{
    constexpr glm::vec3 minBoundaries = glm::vec3(-1.0, -1.0, -1.0);
    constexpr glm::vec3 maxBoundaries = glm::vec3(-minBoundaries);

    constexpr glm::vec3 center         = glm::vec3(0, 0, 0);
    constexpr int       pointsPerCloud = 300;
    // constexpr float     scale          = 0.5f;
    constexpr float     scale          = 1.0f;
    // constexpr float     scale          = 2.0f;
    // constexpr float     scale          = 3.0f;
    // auto*               pointCloud     = new Geometry::PointCloud;
    auto* pointCloud = new Geometry::PointCloud3d;

    for(int idx = 0; idx < pointsPerCloud; ++idx)
    {
        glm::vec3 rand;

        // Default
        // rand = Utils::Random::getUniformRandom(-1.0f, 1.0f) / scale + center;
        // rand.x = Utils::Random::getUniformRandom(minBoundaries.x, maxBoundaries.x) / scale;
        // rand.y = Utils::Random::getUniformRandom(minBoundaries.y, maxBoundaries.y) / scale;
        // rand.z = Utils::Random::getUniformRandom(minBoundaries.z, maxBoundaries.z) / scale;

        // Hemispheric point cloud
        // rand = Utils::Random::getUniformRandomInHemisphere(glm::vec3(0, -1, 0)) / scale + center;
        // rand = Utils::Random::getUniformRandomCosineDirection() / scale + center;

        // Spheric point cloud
        // rand = Utils::Random::getUniformRandomInUnitSphere() / scale + center;
        // rand = Utils::Random::getRandomInUnitSphereSurface() / scale + center;
        // rand = Utils::Random::getUniformRandomInUnitSphereSurface() / scale + center;

        // Disk point cloud
        // rand = Utils::Random::getUniformRandomInUnitDiskCircumference() / scale + center;

        // Disk point cloud
        // rand = Utils::Random::getUniformRandomInUnitDisk() / scale + center;

        // Rectangular point cloud
        // rand = Utils::Random::getUniformRandomInUnitSquare() / scale + center;
        // rand = Utils::Random::getUniformRandomInUnitSquarePerimeter() / scale + center;

        // Cubical point cloud
        rand = Utils::Random::getUniformRandomInUnitCubeSurface() / scale + center;

        // pointCloud->addPoint(Geometry::Point(rand.x, rand.y));
        // pointCloud->addPoint(Geometry::Vect3d(rand.x, rand.y, 0.0));
        pointCloud->addPoint(Geometry::Vect3d(rand.x, rand.y, rand.z));

        auto color = Utils::Random::getUniformRandomColor();
        // auto color = Utils::Random::getUniformRandomColorEuclideanDistance();
        // auto color = glm::vec3(1, 1, 1);
        // sc.addNewModel((new DrawPointCloud(*pointCloud, true))->overrideModelName()->setLineColor(color)->setLineWidth(3.0f)->setPointColor(color)->setPointSize(0.0f));
        // sc.addNewModel((new DrawPointCloud(*pointCloud, true))->overrideModelName()->setLineColor(color)->setLineWidth(3.0f));
        sc.addNewModel((new DrawPointCloud(*pointCloud, false))->overrideModelName()->setPointColor(color)->setPointSize(3.0f));
    }
    {
        auto* segment = new Geometry::Segment3d({1.0 / scale, 1.0 / scale, 1.0 / scale}, {-1.0 / scale, 1.0 / scale, 1.0 / scale});
        sc.addNewModel((new DrawSegment(*segment))->overrideModelName()->setLineColor(glm::vec3(1, 1, 1))->setLineWidth(3.0f));
        delete segment;
    }
    {
        auto* segment = new Geometry::Segment3d({1.0 / scale, 1.0 / scale, 1.0 / scale}, {1.0 / scale, -1.0 / scale, 1.0 / scale});
        sc.addNewModel((new DrawSegment(*segment))->overrideModelName()->setLineColor(glm::vec3(1, 1, 1))->setLineWidth(3.0f));
        delete segment;
    }
    {
        auto* segment = new Geometry::Segment3d({1.0 / scale, 1.0 / scale, 1.0 / scale}, {1.0 / scale, 1.0 / scale, -1.0 / scale});
        sc.addNewModel((new DrawSegment(*segment))->overrideModelName()->setLineColor(glm::vec3(1, 1, 1))->setLineWidth(3.0f));
        delete segment;
    }
    {
        auto* segment = new Geometry::Segment3d({1.0 / scale, 1.0 / scale, -1.0 / scale}, {-1.0 / scale, 1.0 / scale, -1.0 / scale});
        sc.addNewModel((new DrawSegment(*segment))->overrideModelName()->setLineColor(glm::vec3(1, 1, 1))->setLineWidth(3.0f));
        delete segment;
    }
    {
        auto* segment = new Geometry::Segment3d({-1.0 / scale, 1.0 / scale, -1.0 / scale}, {-1.0 / scale, 1.0 / scale, 1.0 / scale});
        sc.addNewModel((new DrawSegment(*segment))->overrideModelName()->setLineColor(glm::vec3(1, 1, 1))->setLineWidth(3.0f));
        delete segment;
    }
    {
        auto* segment = new Geometry::Segment3d({-1.0 / scale, 1.0 / scale, 1.0 / scale}, {-1.0 / scale, -1.0 / scale, 1.0 / scale});
        sc.addNewModel((new DrawSegment(*segment))->overrideModelName()->setLineColor(glm::vec3(1, 1, 1))->setLineWidth(3.0f));
        delete segment;
    }
    {
        auto* segment = new Geometry::Segment3d({-1.0 / scale, -1.0 / scale, 1.0 / scale}, {1.0 / scale, -1.0 / scale, 1.0 / scale});
        sc.addNewModel((new DrawSegment(*segment))->overrideModelName()->setLineColor(glm::vec3(1, 1, 1))->setLineWidth(3.0f));
        delete segment;
    }
    {
        auto* segment = new Geometry::Segment3d({1.0 / scale, -1.0 / scale, 1.0 / scale}, {1.0 / scale, -1.0 / scale, -1.0 / scale});
        sc.addNewModel((new DrawSegment(*segment))->overrideModelName()->setLineColor(glm::vec3(1, 1, 1))->setLineWidth(3.0f));
        delete segment;
    }
    {
        auto* segment = new Geometry::Segment3d({1.0 / scale, -1.0 / scale, -1.0 / scale}, {1.0 / scale, 1.0 / scale, -1.0 / scale});
        sc.addNewModel((new DrawSegment(*segment))->overrideModelName()->setLineColor(glm::vec3(1, 1, 1))->setLineWidth(3.0f));
        delete segment;
    }
    {
        auto* segment = new Geometry::Segment3d({1.0 / scale, -1.0 / scale, -1.0 / scale}, {-1.0 / scale, -1.0 / scale, -1.0 / scale});
        sc.addNewModel((new DrawSegment(*segment))->overrideModelName()->setLineColor(glm::vec3(1, 1, 1))->setLineWidth(3.0f));
        delete segment;
    }
    {
        auto* segment = new Geometry::Segment3d({-1.0 / scale, -1.0 / scale, -1.0 / scale}, {-1.0 / scale, 1.0 / scale, -1.0 / scale});
        sc.addNewModel((new DrawSegment(*segment))->overrideModelName()->setLineColor(glm::vec3(1, 1, 1))->setLineWidth(3.0f));
        delete segment;
    }
    {
        auto* segment = new Geometry::Segment3d({-1.0 / scale, -1.0 / scale, -1.0 / scale}, {-1.0 / scale, -1.0 / scale, 1.0 / scale});
        sc.addNewModel((new DrawSegment(*segment))->overrideModelName()->setLineColor(glm::vec3(1, 1, 1))->setLineWidth(3.0f));
        delete segment;
    }

    // auto* pc2 = new Geometry::PointCloud3d;
    // pc2->addPoint({-1.0, -1.0, -1.0});
    // pc2->addPoint({-1.0, -1.0, 1.0});
    // pc2->addPoint({-1.0, 1.0, -1.0});
    // pc2->addPoint({-1.0, 1.0, 1.0});
    // pc2->addPoint({1.0, -1.0, -1.0});
    // pc2->addPoint({1.0, -1.0, 1.0});
    // pc2->addPoint({1.0, 1.0, -1.0});
    // pc2->addPoint({1.0, 1.0, 1.0});

    // pc2->addPoint({1.0, 1.0, 1.0});
    // pc2->addPoint({-1.0, 1.0, 1.0});
    // pc2->addPoint({1.0, 1.0, 1.0});
    // pc2->addPoint({1.0, -1.0, 1.0});
    // pc2->addPoint({1.0, 1.0, 1.0});
    // pc2->addPoint({1.0, 1.0, -1.0});
    // pc2->addPoint({1.0, 1.0, -1.0});
    // pc2->addPoint({-1.0, 1.0, -1.0});
    // pc2->addPoint({-1.0, 1.0, -1.0});
    // pc2->addPoint({-1.0, 1.0, 1.0});
    // pc2->addPoint({-1.0, 1.0, 1.0});
    // pc2->addPoint({-1.0, -1.0, 1.0});
    // pc2->addPoint({-1.0, -1.0, 1.0});
    // pc2->addPoint({1.0, -1.0, 1.0});
    // pc2->addPoint({1.0, -1.0, 1.0});
    // pc2->addPoint({1.0, -1.0, -1.0});
    // pc2->addPoint({1.0, -1.0, -1.0});
    // pc2->addPoint({1.0, 1.0, -1.0});
    // pc2->addPoint({1.0, -1.0, -1.0});
    // pc2->addPoint({-1.0, -1.0, -1.0});
    // pc2->addPoint({-1.0, -1.0, -1.0});
    // pc2->addPoint({-1.0, 1.0, -1.0});
    // pc2->addPoint({-1.0, -1.0, -1.0});
    // pc2->addPoint({-1.0, -1.0, 1.0});

    // const Geometry::ConvexHull convexHull3D(pc2);
    // const Geometry::ConvexHull convexHull3D(pointCloud);
    // for(auto triangle : convexHull3D.getTriangles3D())
    // {
    //     sc.addNewModel((new DrawTriangle(triangle))->setLineColor(Utils::Random::getUniformRandomColor())->setTriangleColor(glm::vec4(Utils::Random::getUniformRandomColor(), 1.0f))->overrideModelName());
    // }
}
}    // namespace GDSA::Render::Scenes
