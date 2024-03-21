#pragma once

#include "BasicGeometry.h"

#include <climits>
#include <ostream>
#include <sstream>

#include <glm/vec3.hpp>

namespace GDSA::Geometry
{
/**
 *  @brief This class represents a 2D structure. This will be also the skeleton for other 2D structures such as Vertex or Vector.
 */
class Point
{
protected:
    static constexpr int DEFAULT_VALUE = INT_MAX;    // Value of X and Y coordinates for an incomplete Point.

public:
    enum class PointClassification
    {
        LEFT,
        RIGHT,
        FORWARD,
        BACKWARD,
        BETWEEN,
        ORIGIN,
        DEST
    };

public:
    double _x, _y;

    Point();
    Point(double x, double y, bool polar = false);
    Point(const Point& point);

    virtual ~Point() = default;

    Point& operator=(const Point& point) = default;

    /**
     *  @brief Checks the position of the point respect to other two points (a, b).
     */
    bool backward(Point& a, Point& b) const;

    /**
     *  @brief Determines the relative position with respect to other two (which can form a segment).
     */
    PointClassification classify(const Point& p0, Point& p1) const;

    /**
     *  @brief Checks if this point lies on the same line described by a and b.
     */
    bool colinear(Point& a, Point& b) const;

    /**
     *  @brief Distance between this point and another one.
     */
    double distance(const Point& p) const;

    /**
     *  @brief Checks if the coordinates of this point are different from the coordinates of a point p.
     */
    bool distinct(const Point& p) const;

    /**
     *  @brief Checks if the coordinates of this point are equal from the coordinates of a point p.
     */
    bool equal(const Point& p) const;

    /**
     *  @brief Checks the position of the point respect to other two points (a, b).
     */
    bool forward(Point& a, Point& b) const;

    /**
     *  @return X coordinate.
     */
    virtual double getX();

    /**
     *  @return Y coordinate.
     */
    virtual double getY();

    /**
     *  @brief Angle of this point interpreted as a polar coordinate (radians).
     */
    double getAlpha() const;

    /**
     *  @brief Module of a 2D structure. This method is useful for child classes.
     */
    double getModule() const;

    /**
     *  @brief Checks the position of the point respect to other two points (a, b).
     */
    bool isBetween(Point& a, Point& b) const;

    /**
     *  @brief Checks the value of the coordinates. If the values are the DEFAULT, then the point is not valid.
     */
    bool isValid() const;

    /**
     *  @brief Checks the position of the point respect to other two points (a, b).
     */
    bool left(Point& a, Point& b) const;

    /**
     *  @brief Checks the position of the point respect to other two points (a, b).
     */
    bool leftAbove(Point& a, Point& b) const;

    /**
     *  @brief Substraction operator (override).
     */
    virtual Point& operator-(const Point& point);

    /**
     *  @brief Substraction operator (override).
     */
    virtual Point& operator+(const Point& point);

    /**
     *  @brief Substraction operator (override).
     */
    virtual Point& operator*(double scalar);

    /**
     *  @brief Overriding cout call.
     */
    friend std::ostream& operator<<(std::ostream& os, const Point& point);

    /**
     *  @brief Overriding cin call.
     */
    friend std::istream& operator>>(std::istream& is, Point& point);

    /**
     *  @brief Checks the position of the point respect to other two points (a, b).
     */
    bool rightAbove(Point& a, Point& b) const;

    /**
     *  @brief Checks the position of the point respect to other two points (a, b).
     */
    bool right(Point& a, Point& b) const;

    /**
     *  @brief Modifies the coordinate values.
     */
    void set(double x, double y);

    /**
     *  @brief Modifies the X coordinate.
     */
    void setX(double x);

    /**
     *  @brief Modifies the Y coordinate.
     */
    void setY(double y);

    /**
     *  @brief Returns the slope between this point and p.
     */
    double slope(const Point& p) const;

    /**
     *  @brief Calculates the double area of the triangle formed by (this, a, b).
     */
    double triangleArea2(Point& a, Point& b) const;

    /**
     *  @brief Calculates the scalar multiplication of this point.
     */
    Point scalarMult(double t);

    /**
     * @brief Returns the point as a glm::vec3.
     */
    glm::vec3 toGlmVec3() const;
};
}    // namespace GDSA::Geometry
