#pragma once

#include "AABB.h"
#include "Ray3d.h"
#include "Vect3d.h"

/**
 *  @brief This class represents a triangle defined by 3 points.
 */
class Triangle3d
{
public:
    friend class DrawTriangle3d;

    enum PointPosition
    {
        POSITIVE,
        NEGATIVE,
        COPLANAR
    };

    enum PointTrianglePosition
    {
        PARALELL,
        COLLINEAR,
        INTERSECTS,
        NO_INTERSECTS
    };

    // Contains all those attributes necessary for resolving the intersection between a triangle and an AABB.
    struct TriangleAABBIntersData
    {
        Vect3d _v0, _v1, _v2;
        double _min, _max, _rad;
        Vect3d _normal, _edge0, _edge1, _edge2, _p, _fedge;
        Vect3d _boxCenter;
        Vect3d _boxRadius;
    };

protected:
    Vect3d _a, _b, _c;

public:
    /**
     *  @brief Default constructor.
     */
    Triangle3d();

    /**
     *  @brief Constructor.
     */
    Triangle3d(double ax, double ay, double az, double bx, double by, double bz, double cx, double cy, double cz);

    /**
     *  @brief Copy constructor.
     */
    Triangle3d(const Triangle3d& triangle);

    /**
     *  @brief Constructor.
     */
    Triangle3d(Vect3d& va, Vect3d& vb, Vect3d& vc);

    /**
     *  @brief Destructor.
     */
    virtual ~Triangle3d();

    /**
     *  @brief Returns the first point.
     */
    Vect3d getA();

    /**
     *  @brief Returns the second point.
     */
    Vect3d getB();

    /**
     *  @brief Returns the third point.
     */
    Vect3d getC();

    /**
     *  @brief Modifies the first point.
     */
    void setA(const Vect3d& pa);

    /**
     *  @brief Modifies the second point.
     */
    void setB(const Vect3d& pb);

    /**
     *  @brief Modifies the third point.
     */
    void setC(const Vect3d& pc);

    /**
     *  @brief Modifies all the points.
     */
    void set(const Vect3d& va, const Vect3d& vb, const Vect3d& vc);

    /**
     *  @brief Returns the are of the triangle.
     */
    double area() const;

    /**
     *  @brief Returns the position of the point respect to the triangle plane.
     */
    PointPosition classify(const Vect3d& point);

    /**
     *  @brief Returns a new triangle with the same values than this one.
     */
    Triangle3d copy();

    /**
     *  @brief Returns the normal of the triangle.
     */
    Vect3d normal() const;

    /**
     *  @brief Returns bounding box of the triangle.
     *
     */
    AABB getAABB() const;

    /**
     *  @brief Returns a sample point of the triangle.
     *
     */
    Vect3d samplePoint(double x, double y) const;

    /**
     * \brief Ray-Triangle Moller intersection algorithm.
     */
    bool intersect(Ray3d& ray, Vect3d& intersectionPoint);

    /**
     *  @brief Assignment operator.
     *  https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
     */
    Triangle3d& operator=(const Triangle3d& triangle);

    /**
     *  @brief Cout overloading.
     */
    friend std::ostream& operator<<(std::ostream& os, const Triangle3d& triangle);
};
