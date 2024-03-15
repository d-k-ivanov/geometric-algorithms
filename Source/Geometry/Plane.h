#pragma once

#include "Line3d.h"
#include "Segment3d.h"
#include "Vect3d.h"

/*
 *  @brief This class a represents a 3D plane represented by three points.
 */
class Plane
{
public:
    enum IntersectionType
    {
        POINT,
        SEGMENT,
        COPLANAR
    };

public:
    class IntersectionLine
    {
        Vect3d           _point;
        IntersectionType _type;
    };

protected:
    Vect3d _a, _b, _c, _d;

public:

    /**
     *  @param p in pi = p + u * lambda + v * mu -> r from the points (R, S, T).
     *  @param u in pi = p + u * lambda + v * mu -> d from the points (R, S, T).
     *  @param v in pi = p + u * lambda + v * mu -> t from the points (R, S, T).
     *  @param arePoints If arePoints is false, then params are p + u * lambda + v * mu, otherwise are points (R, S, T).
     */
    Plane(const Vect3d& p, const Vect3d& u, const Vect3d& v, bool arePoints);


    /**
     *  @brief Copy constructor.
     */
    Plane(const Plane& plane);

    /**
     *  @brief Destructor.
     */
    virtual ~Plane();

    /**
     *  @brief Returns member A.
     */
    Vect3d& get_A();

    /**
     *  @brief Returns member B.
     */
    Vect3d& get_B();

    /**
     *  @brief Returns member C.
     */
    Vect3d& get_C();

    /**
     *  @brief Returns member D.
     */
    Vect3d& get_D();

    /**
     *  @brief Returns A in AX + BY + CZ + D = 0.
     */
    double getA() const;

    /**
     *  @brief Returns B in AX + BY + CZ + D = 0.
     */
    double getB() const;

    /**
     *  @brief Returns C in AX + BY + CZ + D = 0.
     */
    double getC() const;

    /**
     *  @return D in AX + BY + CZ + D = 0.
     */
    double getD() const;

    /**
     *  @brief Returns the normal vector of (A, B, C) in Ax + By + Cz + D = 0.
     */
    Vect3d getNormal() const;

    /**
     *  @brief Returns the center of the plane.
     */
    Vect3d getCenter() const;

    /**
     *  @brief Returns true if p is in the plane.
     */
    bool coplanar(const Vect3d& point) const;

    /**
     *  @brief Distance between the plane and the point.
     */
    double distance(const Vect3d& point) const;

    /**
     *  @brief Calculates the intersection line of a plane with this plane.
     */
    bool intersect(const Plane& plane, Line3d& intersectionLine) const;

    /**
     *  @brief Calculates the intersection point of a line and this plane, if exists.
     */
    bool intersect(Line3d& line, Vect3d& intersectionPoint) const;

    /**
     *  @brief Calculates the intersection point of three planes.
     */
    bool intersect(const Plane& planeA, const Plane& planeB, Vect3d& intersectionPoint) const;

    /**
     *  @brief Returns the reflected point of a point in the plane.
     */
    Vect3d reflectedPoint(const Vect3d& p) const;

    /**
     *  @brief Assignment operator.
     */
    Plane& operator=(const Plane& plane);

    /**
     *  @brief Shows the plane values at the debug window.
     */
    friend std::ostream& operator<<(std::ostream& os, const Plane& plane);
};
