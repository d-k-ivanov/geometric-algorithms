#pragma once

#include "AABB.h"
#include "BasicGeometry.h"
#include "EisemannRay.h"
#include "Line3d.h"
#include "Plane.h"
#include "Ray3d.h"
#include "Segment3D.h"
#include "Triangle3d.h"
#include "TriangleModel.h"

/**
 *	@brief Computes the intersection between geometric entities.
 */
namespace Intersections3d
{
/// PLANE - PLANE

/**
 *	@brief Computes the intersection line between two planes, if any.
 *	@param result Intersection of both planes, if returned value is true. Otherwise its value is not valid.
 */
bool intersectPPL(Plane& p1, Plane& p2, Line3d& result);

/// PLANE - LINE

/**
 *	@brief Computes the intersection point between a plane and a line.
 *	@param result Intersection point if plane and line intersect, only if returned value is true. Otherwise its value is not valid.
 */
bool intersectPLV(const Plane& plane, Line3d& line, Vect3d& result);

/// TRIANGLE - AABB

/**
 *	@brief Intersection test between bounding box and triangle.
 *	@return True if both entities intersect.
 */
bool intersectTA(Triangle3d& triangle, const AABB& aabb);

namespace TriangleAABB
{
    // Data generated by Möller intersection algorithm
    struct TriangleAABBIntersData
    {
        Vect3d _v0, _v1, _v2;
        double _min, _max, _rad;
        Vect3d _normal, _edge0, _edge1, _edge2, _p, _fedge;
        Vect3d _boxCenter;
        Vect3d _boxRadius;
    };

    /// Möller algorithm.

    /**
     *	@brief Finds the minimum and maximum values of 3 doubleing points.
     */
    void findMinMax(double x0, double x1, double x2, double& min, double& max);

    /**
     *	@brief Checks if a plane and the bounding box overlap.
     */
    bool planeBoxOverlap(const Vect3d& normal, const Vect3d& vertex, const Vect3d& maxBox);

    /**
     *	@brief X axis test for a triangle-AABB intersection.
     *	@return false if intersection has been discarded.
     */
    bool xAxisTest_01(TriangleAABBIntersData* data, double a, double b, double fa, double fb);

    /**
     *	@brief X axis test for a triangle-AABB intersection.
     *	@return false if intersection has been discarded.
     */
    bool xAxisTest_2(TriangleAABBIntersData* data, double a, double b, double fa, double fb);

    /**
     *	@brief Y axis test for a triangle-AABB intersection.
     *	@return false if intersection has been discarded.
     */
    bool yAxisTest_02(TriangleAABBIntersData* data, double a, double b, double fa, double fb);

    /**
     *	@brief Y axis test for a triangle-AABB intersection.
     *	@return false if intersection has been discarded.
     */
    bool yAxisTest_1(TriangleAABBIntersData* data, double a, double b, double fa, double fb);

    /**
     *	@brief Z axis test for a triangle-AABB intersection.
     *	@return false if intersection has been discarded.
     */
    bool zAxisTest_12(TriangleAABBIntersData* data, double a, double b, double fa, double fb);

    /**
     *	@brief Z axis test for a triangle-AABB intersection.
     *	@return false if intersection has been discarded.
     */
    bool zAxisTest_0(TriangleAABBIntersData* data, double a, double b, double fa, double fb);
};

/// TRIANGLE - PLANE

/**
 *	@brief Intersection test between triangle and plane.
 *	@param relation Position of triangle respect to plane.
 *	@return True if both entities intersect.
 */
bool intersectTPRST(Triangle3d& triangle, Plane& plane, TriangleModel::Face::FacePlaneRelation& relation, Segment3d& segment, Vect3d& point);

/// TRIANGLE - RAY

/**
 *	@brief Intersection test between a triangle and a ray.
 *	@param point Intersection point, if triangle and ray intersects.
 *	@return True if both entities intersect.
 */
bool intersectTRP(Triangle3d& triangle, Ray3d& ray, Vect3d& point);

/// AABB - AABB

/**
 *	@brief Intersection test between two axis aligned bounding boxes.
 *	@return True if both entities intersect.
 */
bool intersectAA(const AABB& aabb_a, const AABB& aabb_b);

//// AABB - RAY

/**
 *	@brief Intersection test between an axis-aligned bounding box and a ray.
 *	@return True if both entities intersect.
 */
bool intersectAR(AABB& aabb, const EisemannRay& ray);
};

/// PLANE - PLANE

inline bool Intersections3d::intersectPPL(Plane& p1, Plane& p2, Line3d& result)
{
    const Vect3d n1 = Vect3d(p1.getA(), p1.getB(), p1.getC());
    const Vect3d n2 = Vect3d(p2.getA(), p2.getB(), p2.getC());
    Vect3d       n3 = n1.xProduct(n2);

    // Find the origin of the line
    const double det = BasicGeometry::determinant3x3(n1._x, n1.getY(), n1.getZ(), n2._x, n2.getY(), n2.getZ(), n3._x, n3.getY(), n3.getZ());
    if(BasicGeometry::equal(det, 0.0f))
    {
        result = Line3d();

        return false;
    }

    const double d1 = p1.getD(), d2 = p2.getD();
    const double x0 = (d2 * BasicGeometry::determinant2x2(n1.getY(), n3.getY(), n3.getZ(), n1.getZ()) - d1 * BasicGeometry::determinant2x2(n2.getY(), n3.getY(), n3.getZ(), n2.getZ())) / det;
    const double y0 = (d2 * BasicGeometry::determinant2x2(n3.getX(), n1.getX(), n1.getZ(), n3.getZ()) - d1 * BasicGeometry::determinant2x2(n3.getX(), n2.getX(), n2.getZ(), n3.getZ())) / det;
    const double z0 = (d2 * BasicGeometry::determinant2x2(n1.getX(), n3.getX(), n3.getY(), n1.getY()) - d1 * BasicGeometry::determinant2x2(n2.getX(), n3.getX(), n3.getY(), n2.getY())) / det;

    n3        = n3.normalize();
    Vect3d v1 = Vect3d(x0, y0, z0);
    Vect3d v2 = Vect3d(x0 + n3.getX(), y0 + n3.getY(), z0 + n3.getZ());
    result    = Line3d(v1, v2);

    return true;
}

/// PLANE - LINE

inline bool Intersections3d::intersectPLV(const Plane& plane, Line3d& line, Vect3d& result)
{
    const Vect3d normal = plane.getNormal();
    const double dotDir = normal.dot(line.getDestination() - line.getOrigin());

    if(BasicGeometry::equal(dotDir, 0.0))
    {
        result = Vect3d();

        return false;
    }

    const double lambda = -(normal.dot(line.getOrigin()) + plane.getD()) / dotDir;
    result              = line.getPoint(lambda);

    return true;
}

/// TRIANGLE - AABB

inline bool Intersections3d::intersectTA(Triangle3d& triangle, const AABB& aabb)
{
    Intersections3d::TriangleAABB::TriangleAABBIntersData data;
    data._boxCenter = aabb.getCenter();
    data._boxRadius = aabb.getExtent();

    // Move everything so that the boxcenter is in (0, 0, 0)
    data._v0 = triangle.getA() - data._boxCenter;
    data._v1 = triangle.getB() - data._boxCenter;
    data._v2 = triangle.getC() - data._boxCenter;

    // Compute triangle edges
    data._edge0 = data._v1 - data._v0;
    data._edge1 = data._v2 - data._v1;
    data._edge2 = data._v0 - data._v2;

    // Try the 9 tests first
    data._fedge._x = std::fabsf(data._edge0._x);
    data._fedge._y = std::fabsf(data._edge0._y);
    data._fedge._z = std::fabsf(data._edge0._z);
    if(!Intersections3d::TriangleAABB::xAxisTest_01(&data, data._edge0._z, data._edge0._y, data._fedge._z, data._fedge._y))
        return false;
    if(!Intersections3d::TriangleAABB::yAxisTest_02(&data, data._edge0._z, data._edge0._x, data._fedge._z, data._fedge._x))
        return false;
    if(!Intersections3d::TriangleAABB::zAxisTest_12(&data, data._edge0._y, data._edge0._x, data._fedge._y, data._fedge._x))
        return false;

    data._fedge._x = std::fabsf(data._edge1._x);
    data._fedge._y = std::fabsf(data._edge1._y);
    data._fedge._z = std::fabsf(data._edge1._z);
    if(!Intersections3d::TriangleAABB::xAxisTest_01(&data, data._edge1._z, data._edge1._y, data._fedge._z, data._fedge._y))
        return false;
    if(!Intersections3d::TriangleAABB::yAxisTest_02(&data, data._edge1._z, data._edge1._x, data._fedge._z, data._fedge._x))
        return false;
    if(!Intersections3d::TriangleAABB::zAxisTest_0(&data, data._edge1._y, data._edge1._x, data._fedge._y, data._fedge._x))
        return false;

    data._fedge._x = std::fabsf(data._edge2._x);
    data._fedge._y = std::fabsf(data._edge2._y);
    data._fedge._z = std::fabsf(data._edge2._z);
    if(!Intersections3d::TriangleAABB::xAxisTest_2(&data, data._edge2._z, data._edge2._y, data._fedge._z, data._fedge._y))
        return false;
    if(!Intersections3d::TriangleAABB::yAxisTest_1(&data, data._edge2._z, data._edge2._x, data._fedge._z, data._fedge._x))
        return false;
    if(!Intersections3d::TriangleAABB::zAxisTest_12(&data, data._edge2._y, data._edge2._x, data._fedge._y, data._fedge._x))
        return false;

    /*  First test overlap in the {x,y,z}-directions, find min, max of the triangle for each direction, and test for overlap in
        that direction -- this is equivalent to testing a minimal AABB around the triangle against the AABB */
    Intersections3d::TriangleAABB::findMinMax(data._v0._x, data._v1._x, data._v2._x, data._min, data._max);
    if(data._min > data._boxRadius._x || data._max < -data._boxRadius._x)
        return false;

    Intersections3d::TriangleAABB::findMinMax(data._v0._y, data._v1._y, data._v2._y, data._min, data._max);
    if(data._min > data._boxRadius._y || data._max < -data._boxRadius._y)
        return false;

    Intersections3d::TriangleAABB::findMinMax(data._v0._z, data._v1._z, data._v2._z, data._min, data._max);
    if(data._min > data._boxRadius._z || data._max < -data._boxRadius._z)
        return false;

    /* Test if the box intersects the plane of the triangle: compute plane equation of triangle as normal * x + d = 0 */
    data._normal = data._edge0.xProduct(data._edge1);
    if(!Intersections3d::TriangleAABB::planeBoxOverlap(data._normal, data._v0, data._boxRadius))
        return false;

    return true;
}

inline void Intersections3d::TriangleAABB::findMinMax(const double x0, const double x1, const double x2, double& min, double& max)
{
    min = max = x0;
    if(x1 < min)
        min = x1;
    if(x1 > max)
        max = x1;
    if(x2 < min)
        min = x2;
    if(x2 > max)
        max = x2;
}

inline bool Intersections3d::TriangleAABB::planeBoxOverlap(const Vect3d& normal, const Vect3d& vertex, const Vect3d& maxBox)
{
    double v;
    Vect3d vmin(0.0f), vmax(0.0f);

    for(int q = 0; q <= 2; ++q)
    {
        v = vertex.getAt(q);

        if(normal.getAt(q) > 0.0f)
        {
            vmin.setAt(q, -maxBox.getAt(q) - v);
            vmax.setAt(q, maxBox.getAt(q) - v);
        }
        else
        {
            vmin.setAt(q, maxBox.getAt(q) - v);
            vmax.setAt(q, -maxBox.getAt(q) - v);
        }
    }

    if(normal.dot(vmin) > 0.0f)
        return false;

    if(normal.dot(vmax) >= 0.0f)
        return true;

    return false;
}

inline bool Intersections3d::TriangleAABB::xAxisTest_01(TriangleAABBIntersData* data, double a, double b, double fa, double fb)
{
    data->_p._x = a * data->_v0._y - b * data->_v0._z;
    data->_p._z = a * data->_v2._y - b * data->_v2._z;

    if(data->_p._x < data->_p._z)
    {
        data->_min = data->_p._x;
        data->_max = data->_p._z;
    }
    else
    {
        data->_min = data->_p._z;
        data->_max = data->_p._x;
    }

    data->_rad = fa * data->_boxRadius._y + fb * data->_boxRadius._z;

    if(data->_min > data->_rad || data->_max < -data->_rad)
        return false;

    return true;
}

inline bool Intersections3d::TriangleAABB::xAxisTest_2(TriangleAABBIntersData* data, double a, double b, double fa, double fb)
{
    data->_p._x = a * data->_v0._y - b * data->_v0._z;
    data->_p._y = a * data->_v1._y - b * data->_v1._z;

    if(data->_p._x < data->_p._y)
    {
        data->_min = data->_p._x;
        data->_max = data->_p._y;
    }
    else
    {
        data->_min = data->_p._y;
        data->_max = data->_p._x;
    }

    data->_rad = fa * data->_boxRadius._y + fb * data->_boxRadius._z;

    if(data->_min > data->_rad || data->_max < -data->_rad)
        return false;

    return true;
}

inline bool Intersections3d::TriangleAABB::yAxisTest_02(TriangleAABBIntersData* data, double a, double b, double fa, double fb)
{
    data->_p._x = -a * data->_v0._x + b * data->_v0._z;
    data->_p._z = -a * data->_v2._x + b * data->_v2._z;

    if(data->_p._x < data->_p._z)
    {
        data->_min = data->_p._x;
        data->_max = data->_p._z;
    }
    else
    {
        data->_min = data->_p._z;
        data->_max = data->_p._x;
    }

    data->_rad = fa * data->_boxRadius._x + fb * data->_boxRadius._z;

    if(data->_min > data->_rad || data->_max < -data->_rad)
        return false;

    return true;
}

inline bool Intersections3d::TriangleAABB::yAxisTest_1(TriangleAABBIntersData* data, double a, double b, double fa, double fb)
{
    data->_p._x = -a * data->_v0._x + b * data->_v0._z;
    data->_p._y = -a * data->_v1._x + b * data->_v1._z;

    if(data->_p._x < data->_p._y)
    {
        data->_min = data->_p._x;
        data->_max = data->_p._y;
    }
    else
    {
        data->_min = data->_p._y;
        data->_max = data->_p._x;
    }

    data->_rad = fa * data->_boxRadius._x + fb * data->_boxRadius._z;

    if(data->_min > data->_rad || data->_max < -data->_rad)
        return false;

    return true;
}

inline bool Intersections3d::TriangleAABB::zAxisTest_12(TriangleAABBIntersData* data, double a, double b, double fa, double fb)
{
    data->_p._y = a * data->_v1._x - b * data->_v1._y;
    data->_p._z = a * data->_v2._x - b * data->_v2._y;

    if(data->_p._z < data->_p._y)
    {
        data->_min = data->_p._z;
        data->_max = data->_p._y;
    }
    else
    {
        data->_min = data->_p._y;
        data->_max = data->_p._z;
    }

    data->_rad = fa * data->_boxRadius._x + fb * data->_boxRadius._y;

    if(data->_min > data->_rad || data->_max < -data->_rad)
        return false;

    return true;
}

inline bool Intersections3d::TriangleAABB::zAxisTest_0(TriangleAABBIntersData* data, double a, double b, double fa, double fb)
{
    data->_p._x = a * data->_v0._x - b * data->_v0._y;
    data->_p._y = a * data->_v1._x - b * data->_v1._y;

    if(data->_p._x < data->_p._y)
    {
        data->_min = data->_p._x;
        data->_max = data->_p._y;
    }
    else
    {
        data->_min = data->_p._y;
        data->_max = data->_p._x;
    }

    data->_rad = fa * data->_boxRadius._x + fb * data->_boxRadius._y;

    if(data->_min > data->_rad || data->_max < -data->_rad)
        return false;

    return true;
}

/// TRIANGLE - PLANE

inline bool Intersections3d::intersectTPRST(Triangle3d& triangle, Plane& plane, TriangleModel::Face::FacePlaneRelation& relation, Segment3d& segment, Vect3d& point)
{
    const Vect3d pointOnPlane = plane.getPoint(0.0f, 0.0f);
    const Vect3d planeNormal  = plane.getNormal();
    Vect3d       a = triangle.getA(), b = triangle.getB(), c = triangle.getC();
    double       dot1 = planeNormal.dot(triangle.getA().sub(pointOnPlane));
    double       dot2 = planeNormal.dot(triangle.getB().sub(pointOnPlane));
    double       dot3 = planeNormal.dot(triangle.getC().sub(pointOnPlane));

    if(BasicGeometry::equal(dot1, 0.0f))
        dot1 = 0.0f;
    if(BasicGeometry::equal(dot2, 0.0f))
        dot2 = 0.0f;
    if(BasicGeometry::equal(dot3, 0.0f))
        dot3 = 0.0f;

    // All points above plane
    if(dot1 > 0.0f && dot2 > 0.0f && dot3 > 0.0f)
    {
        relation = TriangleModel::Face::FacePlaneRelation::POSITIVE;

        return false;
    }

    // All points below plane
    if(dot1 < 0.0f && dot2 < 0.0f && dot3 < 0.0f)
    {
        relation = TriangleModel::Face::FacePlaneRelation::NEGATIVE;

        return false;
    }

    // Coplanar case
    if(BasicGeometry::equal((std::abs(dot1) + std::abs(dot2) + std::abs(dot3)), 0.0f))
    {
        relation = TriangleModel::Face::FacePlaneRelation::COPLANAR;

        return true;
    }

    // Most common intersection
    Vect3d point1, point2;
    Line3d line1, line2;
    bool   caseA = false, caseB = false, caseC = false, caseD = true;

    if((dot1 > 0.0f && dot2 > 0.0f && dot3 < 0.0f) || (dot1 < 0.0f && dot2 < 0.0f && dot3 > 0.0f))
    {
        line1 = Line3d(a, c);
        line2 = Line3d(b, c);
        caseA = true;
    }

    if((dot2 > 0.0f && dot3 > 0.0f && dot1 < 0.0f) || (dot2 < 0.0f && dot3 < 0.0f && dot1 > 0.0f))
    {
        line1 = Line3d(b, a);
        line2 = Line3d(c, a);
        caseA = true;
    }

    if((dot1 > 0.0f && dot3 > 0.0f && dot2 < 0.0f) || (dot1 < 0.0f && dot3 < 0.0f && dot2 > 0.0f))
    {
        line1 = Line3d(a, b);
        line2 = Line3d(c, b);
        caseA = true;
    }

    if(caseA)
    {
        intersectPLV(plane, line1, point1);
        intersectPLV(plane, line2, point2);

        relation = TriangleModel::Face::FacePlaneRelation::SEGMENT_INT;
        segment  = Segment3d(point1, point2);

        return true;
    }

    // Case B
    if(dot1 == 0 && ((dot2 > 0 && dot3 > 0) || (dot2 < 0 && dot3 < 0)))
    {
        point1 = a;
        caseB  = true;
    }

    if(dot2 == 0 && ((dot1 > 0 && dot3 > 0) || (dot1 < 0 && dot3 < 0)))
    {
        point1 = b;
        caseB  = true;
    }

    if(dot3 == 0 && ((dot2 > 0 && dot1 > 0) || (dot2 < 0 && dot1 < 0)))
    {
        point1 = c;
        caseB  = true;
    }

    if(caseB)
    {
        relation = TriangleModel::Face::FacePlaneRelation::POINT_INT;
        point    = point1;
    }

    // Case C
    if(dot1 == 0 && ((dot2 > 0 && dot3 < 0) || (dot2 < 0 && dot2 > 0)))
    {
        line1  = Line3d(c, b);
        point2 = a;
        caseC  = true;
    }

    if(dot2 == 0 && ((dot1 > 0 && dot3 < 0) || (dot1 < 0 && dot3 > 0)))
    {
        line1  = Line3d(a, c);
        point2 = b;
        caseC  = true;
    }

    if(dot3 == 0 && ((dot1 > 0 && dot2 < 0) || (dot1 < 0 && dot2 > 0)))
    {
        line1  = Line3d(a, b);
        point2 = c;
        caseC  = true;
    }

    if(caseC)
    {
        intersectPLV(plane, line1, point1);

        relation = TriangleModel::Face::FacePlaneRelation::SEGMENT_INT;
        segment  = Segment3d(point2, point1);

        return true;
    }

    // Case D
    if(dot1 == 0 && dot2 == 0)
    {
        point1 = a;
        point2 = b;
        caseD  = true;
    }

    if(dot2 == 0 && dot3 == 0)
    {
        point1 = b;
        point2 = c;
        caseD  = true;
    }

    if(dot1 == 0 && dot3 == 0)
    {
        point1 = a;
        point2 = c;
        caseD  = true;
    }

    if(caseD)
    {
        relation = TriangleModel::Face::FacePlaneRelation::SEGMENT_INT;
        segment  = Segment3d(point1, point2);

        return true;
    }

    relation = TriangleModel::Face::FacePlaneRelation::NO_RELATION;

    return false;
}

/// TRIANGLE - RAY

inline bool Intersections3d::intersectTRP(Triangle3d& triangle, Ray3d& ray, Vect3d& point)
{
    Vect3d edge1, edge2, rayVector, h, s, q;
    double a, f, u, v;

    edge1     = triangle.getB() - triangle.getA();
    edge2     = triangle.getB() - triangle.getA();
    rayVector = ray.getDestination().sub(ray.getOrigin()).normalize();

    h = rayVector.xProduct(edge2);
    a = edge1.dot(h);

    if(BasicGeometry::equal(a, 0.0f))    // Parallel ray case
    {
        return false;
    }

    f = 1.0f / a;
    s = ray.getOrigin() - triangle.getA();
    u = f * s.dot(h);

    if(u < 0.0f || u > 1.0f)
    {
        return false;
    }

    q = s.xProduct(edge1);
    v = f * rayVector.dot(q);

    if(v < 0.0f || ((u + v) > 1.0f))
    {
        return false;
    }

    double t = f * edge2.dot(q);
    if(t > BasicGeometry::EPSILON)
    {
        point = ray.getOrigin() + rayVector * t;

        return true;
    }

    return false;    // There is a LINE intersection but no RAY intersection
}

/// AABB - AABB

/**
 *	@brief Intersection test between two axis aligned bounding boxes.
 *	@return True if both entities intersect.
 */
inline bool Intersections3d::intersectAA(const AABB& aabb_a, const AABB& aabb_b)
{
    const Vect3d aCenter = aabb_a.getCenter(), aExtent = aabb_a.getExtent();
    const Vect3d bCenter = aabb_b.getCenter(), bExtent = aabb_b.getExtent();

    const bool x = std::abs(aCenter._x - bCenter._x) <= (aExtent._x + bExtent._x);
    const bool y = std::abs(aCenter._y - bCenter._y) <= (aExtent._y + bExtent._y);
    const bool z = std::abs(aCenter._z - bCenter._z) <= (aExtent._z + bExtent._z);

    return x && y && z;    // Overlap!
}

/// AABB - RAY

inline bool Intersections3d::intersectAR(AABB& aabb, const EisemannRay& ray)
{
    return ray.intersect(aabb);
}
