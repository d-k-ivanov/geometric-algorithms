#pragma once

#include "SceneContent.h"

class Point;

namespace AlgGeom
{
class Scenes
{
public:
    // Practice 0:
    static void p0(SceneContent& sc);
    static void p0a(SceneContent& sc);
    static void p0b(SceneContent& sc);

    // Practice 1:
    static void p1PointClouds(SceneContent& sc, int numPointClouds, int pointsPerCloud, float scaleFactor, std::vector<Point>& randomPointsFromCloud, std::vector<Point>& extremumPointInCloud);
    static void p1Lines(SceneContent& sc, const std::vector<Point>& randomPointsFromCloud);
    static void p1Polygon(SceneContent& sc, const std::vector<Point>& extremumPointInCloud);
    static void p1Bezier(SceneContent& sc, bool randomPoints = false, size_t pointNum = 4);
    static void p1Intersections(SceneContent& sc);
    static void p1All(SceneContent& sc);

    // Practice 2:
    static void p2a(SceneContent& sc, int numPointClouds, int pointsPerCloud, float scaleFactor);
    static void p2b(SceneContent& sc);
    static void p2c(SceneContent& sc);

    // Practice 3:
    static void p3(SceneContent& sc);

    // Practice 4:
    static void p4a(SceneContent& sc, bool drawTriangles);
    static void p4b(SceneContent& sc, bool randomOnSphereSurface);
    static void p4c(SceneContent& sc);
    static void p4d(SceneContent& sc);

};
}    // namespace AlgGeom