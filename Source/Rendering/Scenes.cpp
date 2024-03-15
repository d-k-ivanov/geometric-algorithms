#include "Scenes.h"

#include "DrawAABB.h"
#include "DrawBezier.h"
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
#include "Geometry/Plane.h"
#include "Geometry/PointCloud.h"
#include "Geometry/Polygon.h"
#include "Geometry/SegmentLine.h"
#include "Geometry/TriangleModel.h"

#include "Utils/ChronoUtilities.h"
#include "Utils/FilesystemUtilities.h"
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

    constexpr int   numTriangles = 30;
    constexpr float alpha        = 2 * glm::pi<float>() / static_cast<float>(numTriangles);

    for(int triangleIdx = 0; triangleIdx < numTriangles; ++triangleIdx)
    {
        const float randB = RandomUtilities::getUniformRandom(.5f, .9f);
        const float randC = RandomUtilities::getUniformRandom(.6f, .8f);
        Vect2d      a(.0f, .0f);
        Vect2d      b(glm::cos(alpha * static_cast<float>(triangleIdx)) * randB, glm::sin(alpha * static_cast<float>(triangleIdx)) * randB);
        Vect2d      c(glm::cos(alpha * static_cast<float>(triangleIdx + 1)) * randC, glm::sin(alpha * static_cast<float>(triangleIdx + 1)) * randC);
        Triangle*   triangle = new Triangle(a, b, c);

        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
}

void AlgGeom::Scenes::p0a(SceneContent& sc)
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);

    {
        Vect3d      a {0.5f, -0.5f, 0.0f};
        Vect3d      b {0.5f, -0.5f, 0.0f};
        Vect3d      c {0.0f, 0.5f, 0.0f};
        Triangle3d* triangle = new Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Vect3d      a {-0.5f, -0.5f, 0.0f};
        Vect3d      b {0.0f, -0.5f, 0.5f};
        Vect3d      c {0.0f, 0.5f, 0.0f};
        Triangle3d* triangle = new Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Vect3d      a {-0.5f, -0.5f, 0.0f};
        Vect3d      b {0.0f, -0.5f, -0.5f};
        Vect3d      c {0.0f, 0.5f, 0.0f};
        Triangle3d* triangle = new Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Vect3d      a {0.5f, -0.5f, 0.0f};
        Vect3d      b {0.0f, -0.5f, 0.5f};
        Vect3d      c {0.0f, 0.5f, 0.0f};
        Triangle3d* triangle = new Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Vect3d      a {0.5f, -0.5f, 0.0f};
        Vect3d      b {0.0f, -0.5f, -0.5f};
        Vect3d      c {0.0f, 0.5f, 0.0f};
        Triangle3d* triangle = new Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Vect3d      a {-0.5f, -0.5f, 0.0f};
        Vect3d      b {0.5f, -0.5f, 0.0f};
        Vect3d      c {0.0f, -0.5f, -0.5f};
        Triangle3d* triangle = new Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
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
            center = glm::vec3(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y), 0.0f);
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

void AlgGeom::Scenes::p2a(SceneContent& sc, int numPointClouds, int pointsPerCloud, float scaleFactor)
{
    constexpr glm::vec2 minBoundaries = glm::vec2(-3.0, -1.5);
    constexpr glm::vec2 maxBoundaries = glm::vec2(-minBoundaries);

    // Tasks
    // Pr2-a-1: point cloud
    glm::vec3 center;
    for(int pcIdx = 0; pcIdx < numPointClouds; ++pcIdx)
    {
        auto* pointCloud = new PointCloud3d;

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
                RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x),
                RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y),
                RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
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
            pointCloud->addPoint({rand.x, rand.y, rand.z});
        }

        sc.addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setPointSize(5.0f));
        // sc.addNewModel((new DrawPointCloud(*pointCloud))->setPointColor(RandomUtilities::getUniformRandomColorEuclideanDistance())->overrideModelName()->setPointSize(7.0f));

        if(pcIdx == 0 and pointCloud->size() >= 2)
        {
            // Line L1
            auto l1A = pointCloud->getPoint(RandomUtilities::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));
            auto l1B = pointCloud->getPoint(RandomUtilities::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));

            // Test parallel:
            // Vect3d l1A(-2, 0, -1);
            // Vect3d l1B(2, 0, -1);

            // Test perpendicular:
            // Vect3d l1A(0, -2, -1);
            // Vect3d l1B(0, 2, -1);
            auto* l1       = new Line3d(l1A, l1B);
            auto  l1Colour = RandomUtilities::getUniformRandomColor();
            sc.addNewModel((new DrawLine(*l1))->setLineColor(l1Colour)->overrideModelName()->setPointSize(5.0f)->setLineWidth(3.0f));
            sc.addNewModel((new DrawPoint(l1A))->setPointColor(l1Colour)->overrideModelName()->setPointSize(10.0f));
            sc.addNewModel((new DrawPoint(l1B))->setPointColor(l1Colour)->overrideModelName()->setPointSize(10.0f));

            // Line L2
            auto l2A = pointCloud->getPoint(RandomUtilities::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));
            auto l2B = pointCloud->getPoint(RandomUtilities::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));

            // Replace to test if lines are parallel or perpendicular.
            // Vect3d l2A(-2, 1, -1);
            // Vect3d l2B(2, 1, -1);
            auto* l2       = new Line3d(l2A, l2B);
            auto  l2Colour = RandomUtilities::getUniformRandomColor();
            sc.addNewModel((new DrawLine(*l2))->setLineColor(l2Colour)->overrideModelName()->setPointSize(5.0f)->setLineWidth(3.0f));
            sc.addNewModel((new DrawPoint(l2A))->setPointColor(l2Colour)->overrideModelName()->setPointSize(10.0f));
            sc.addNewModel((new DrawPoint(l2B))->setPointColor(l2Colour)->overrideModelName()->setPointSize(10.0f));

            // Ray R
            auto rA = pointCloud->getPoint(RandomUtilities::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));
            auto rB = pointCloud->getPoint(RandomUtilities::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));
            // Vect3d rA(-2, 2, -1);
            // Vect3d rB(2, 2, -1);
            auto* r       = new Ray3d(rA, rB);
            auto  rColour = RandomUtilities::getUniformRandomColor();
            sc.addNewModel((new DrawRay(*r))->setLineColor(rColour)->overrideModelName()->setPointSize(5.0f)->setLineWidth(3.0f));
            sc.addNewModel((new DrawPoint(rA))->setPointColor(rColour)->overrideModelName()->setPointSize(10.0f));
            sc.addNewModel((new DrawPoint(rB))->setPointColor(rColour)->overrideModelName()->setPointSize(10.0f));

            // Segment S
            auto sA = pointCloud->getPoint(RandomUtilities::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));
            auto sB = pointCloud->getPoint(RandomUtilities::getUniformRandomInt(0, static_cast<int>(pointCloud->size())));
            // Vect3d sA(-2,-1, -1);
            // Vect3d sB(2, -1, -1);
            auto* s       = new Segment3d(sA, sB);
            auto  sColour = RandomUtilities::getUniformRandomColor();
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

            double maxDistance = 0;
            Vect3d theMostDistantPoint;
            auto   sLine = new Line3d(s->getOrigin(), s->getDestination());
            for(auto& point : pointCloud->getPoints())
            {
                double distance = sLine->distance(point);
                if(distance > maxDistance)
                {
                    maxDistance         = distance;
                    theMostDistantPoint = point;
                }
            }
            sc.addNewModel((new DrawPoint(theMostDistantPoint))->setPointColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setPointSize(20.0f));

            // Line L3
            auto* l3       = new Line3d(l1->normalLine(theMostDistantPoint));
            auto  l3Colour = RandomUtilities::getUniformRandomColor();
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

            AABB* pointCloudAABB = new AABB(pointCloud->getAABB());
            sc.addNewModel((new DrawAABB(*pointCloudAABB))->overrideModelName());

            Vect3d min1(pointCloudAABB->getMin());
            Vect3d min2(pointCloudAABB->getMin().getX(), pointCloudAABB->getMin().getY(), pointCloudAABB->getMax().getZ());
            Vect3d min3(pointCloudAABB->getMax().getX(), pointCloudAABB->getMin().getY(), pointCloudAABB->getMin().getZ());
            Plane* lowerPlane = new Plane(min1, min2, min3, true);
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

void AlgGeom::Scenes::p2b(SceneContent& sc)
{
    constexpr glm::vec3 minBoundaries = glm::vec3(-3.5, -1.5, -2.5);
    constexpr glm::vec3 maxBoundaries = glm::vec3(-minBoundaries);

    const Vect3d pRand(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x),
                       RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y),
                       RandomUtilities::getUniformRandom(minBoundaries.z, maxBoundaries.z));

    const Vect3d uRand(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x),
                       RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y),
                       RandomUtilities::getUniformRandom(minBoundaries.z, maxBoundaries.z));

    const Vect3d vRand(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x),
                       RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y),
                       RandomUtilities::getUniformRandom(minBoundaries.z, maxBoundaries.z));

    // const Vect3d pRand(0, 0, 0);
    // const Vect3d uRand(1, 1, 1);
    // const Vect3d vRand(-1, 1, -1);

    const auto randomColour = RandomUtilities::getUniformRandomColor();
    // sc.addNewModel((new DrawPoint(pRand))->setPointColor(randomColour)->overrideModelName()->setPointSize(10.0f));
    // sc.addNewModel((new DrawPoint(uRand))->setPointColor(randomColour)->overrideModelName()->setPointSize(10.0f));
    // sc.addNewModel((new DrawPoint(vRand))->setPointColor(randomColour)->overrideModelName()->setPointSize(10.0f));

    Plane* randomPlane = new Plane(pRand, uRand, vRand, true);
    sc.addNewModel((new DrawPlane(*randomPlane))->overrideModelName()->setLineWidth(3.0)->setLineColor(glm::vec3(1, 1, 1)));

    const Vect3d vRand2(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x),
                        RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y),
                        RandomUtilities::getUniformRandom(minBoundaries.z, maxBoundaries.z));

    Vect3d newPlaneNormal;
    double newPlaneD;
    pRand.getPlane(uRand, newPlaneNormal, newPlaneD);
    Vect3d newPlaneP1(0, 0, -newPlaneD / newPlaneNormal.getZ());
    Vect3d newPlaneP2(-newPlaneD / newPlaneNormal.getX(), 0, 0);
    Vect3d newPlaneP3(0, -newPlaneD / newPlaneNormal.getY(), 0);
    Plane* newPlane = new Plane(newPlaneP1, newPlaneP2, newPlaneP3, true);
    // sc.addNewModel((new DrawPlane(*newPlane))->overrideModelName()->setLineWidth(3.0)->setLineColor(glm::vec3(0, 1, 1)));

    Line3d intersectionLine;
    if(randomPlane->intersect(*newPlane, intersectionLine))
    {
        sc.addNewModel((new DrawLine(intersectionLine))->overrideModelName()->setLineWidth(5.0)->setLineColor(glm::vec3(1, 1, 0)));
    }
    else
    {
        std::cout << "Planes don't intersect\n";
    }

    Vect3d v1(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x),
              RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y),
              RandomUtilities::getUniformRandom(minBoundaries.z, maxBoundaries.z));

    sc.addNewModel((new DrawPoint(v1))->setPointColor(glm::vec3(0, 0, 1))->overrideModelName()->setPointSize(20.0));
    std::cout << "The distance from V1 to randomPlane: " << randomPlane->distance(v1) << '\n';

    Vect3d v2(randomPlane->reflectedPoint(v1));
    sc.addNewModel((new DrawPoint(v2))->setPointColor(glm::vec3(0, 1, 1))->overrideModelName()->setPointSize(20.0));
    std::cout << "The distance from V2(reflected) to randomPlane: " << randomPlane->distance(v2) << '\n';

    delete randomPlane;
    delete newPlane;
}

void AlgGeom::Scenes::p2c(SceneContent& sc)
{
    constexpr glm::vec3 minBoundaries = glm::vec3(-3.5, -1.5, -2.5);
    constexpr glm::vec3 maxBoundaries = glm::vec3(-minBoundaries);

    TriangleModel* triangleModel = new TriangleModel(ThisExecutableLocation() + "/Resources/Models/Ajax.obj");
    const auto     model(new DrawMesh(*triangleModel));
    model->moveGeometryToOrigin(model->getModelMatrix(), 10.0f)->setModelMatrix(glm::translate(model->getModelMatrix(), glm::vec3(0.0f, 0.0f, 0.0f)));
    model->setModelMatrix(glm::rotate(model->getModelMatrix(), -0.2f, glm::vec3(0.0f, 1.0f, 0.0f)))->overrideModelName();
    sc.addNewModel(model);

    {
        Vect3d      a {0.0, 0.0, 0.0};
        Vect3d      b {2.0, 0.0, 0.0};
        Vect3d      c {1.0, 1.0, 0.0};
        Triangle3d* triangle = new Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Vect3d      a {0.0, 0.0, 0.0};
        Vect3d      b {0.0, 2.0, 0.0};
        Vect3d      c {-1.0, 1.0, 0.0};
        Triangle3d* triangle = new Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Vect3d      a {0.0, 0.0, 0.0};
        Vect3d      b {-2.0, 0.0, 0.0};
        Vect3d      c {-1.0, -1.0, 0.0};
        Triangle3d* triangle = new Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Vect3d      a {0.0, 0.0, 0.0};
        Vect3d      b {0.0, -2.0, 0.0};
        Vect3d      c {1.0, -1.0, 0.0};
        Triangle3d* triangle = new Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Vect3d      a {0.0, 0.0, 0.0};
        Vect3d      b {0.0, 0.0, 2.0};
        Vect3d      c {1.0, 0.0, 1.0};
        Triangle3d* triangle = new Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
    {
        Vect3d      a {0.0, 0.0, 0.0};
        Vect3d      b {0.0, 0.0, -2.0};
        Vect3d      c {1.0, 0.0, -1.0};
        Triangle3d* triangle = new Triangle3d(a, b, c);
        sc.addNewModel((new DrawTriangle(*triangle))->setLineColor(RandomUtilities::getUniformRandomColor())->setTriangleColor(glm::vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
        delete triangle;
    }
}
