#pragma once

#include "SceneContent.h"

class Point;

namespace AlgGeom
{
class Scenes
{
public:
    static void p0(SceneContent& sc);
    ;
    static void p1PointClouds(SceneContent& sc, int numPointClouds, int pointsPerCloud, float scaleFactor, std::vector<Point>& randomPointsFromCloud, std::vector<Point>& extremumPointInCloud);
    static void p1Lines(SceneContent& sc, const std::vector<Point>& randomPointsFromCloud);
    static void p1Polygon(SceneContent& sc, const std::vector<Point>& extremumPointInCloud);
    static void p1Bezier(SceneContent& sc, bool randomPoints = false, size_t pointNum = 4);
    static void p1Intersections(SceneContent& sc);
    static void p1All(SceneContent& sc);

};
}    // namespace AlgGeom