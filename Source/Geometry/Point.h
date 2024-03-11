#pragma once

#include "BasicGeometry.h"

#include <ostream>
#include <sstream>

/**
 *	@brief This class represents a 2D structure. This will be also the skeleton for other 2D structures such as Vertex or Vector.
 */
class Point
{
protected:
    static constexpr int DEFAULT_VALUE = INT_MAX;    // Value of X and Y coordinates for an incomplete Point.

public:
    enum PointClassification
    {
        LEFT,
        RIGHT,
        FORWARD,
        BACKWARD,
        BETWEEN,
        ORIGIN,
        DEST
    };

protected:
    double _x, _y;

public:
    /**
     *	@brief Default constructor. Coordinates are initialized with an invalid value.
     */
    Point();

    /**
     *	@brief Constructor. If polar, x is an angle (radians) and y the vector module.
     */
    Point(double x, double y, bool polar = false);

    /**
     *	@brief Copy constructor.
     */
    Point(const Point& point);

    /**
     *	@brief Destructor.
     */
    virtual ~Point();

    /**
     *	@brief Checks the position of the point respect to other two points (a, b).
     */
    bool backward(Point& a, Point& b);

    /**
     *	@brief Determines the relative position with respect to other two (which can form a segment).
     */
    PointClassification classify(Point& p0, Point& p1);

    /**
     *	@brief Checks if this point lies on the same line described by a and b.
     */
    bool colinear(Point& a, Point& b);

    /**
     *	@brief Distance between this point and another one.
     */
    double distance(Point& p);

    /**
     *	@brief Checks if the coordinates of this point are different from the coordinates of a point p.
     */
    bool distinct(Point& p);

    /**
     *	@brief Checks if the coordinates of this point are equal from the coordinates of a point p.
     */
    bool equal(Point& p);

    /**
     *	@brief Checks the position of the point respect to other two points (a, b).
     */
    bool forward(Point& a, Point& b);

    /**
     *	@return X coordinate.
     */
    virtual double getX();

    /**
     *	@return Y coordinate.
     */
    virtual double getY();

    /**
     *	@brief Angle of this point interpreted as a polar coordinate (radians).
     */
    double getAlpha();

    /**
     *	@brief Module of _a 2D structure. This method is useful for child classes.
     */
    double getModule();

    /**
     *	@brief Checks the position of the point respect to other two points (a, b).
     */
    bool isBetween(Point& a, Point& b);

    /**
     *	@brief Checks the value of the coordinates. If the values are the DEFAULT, then the point is not valid.
     */
    bool isValid();

    /**
     *	@brief Checks the position of the point respect to other two points (a, b).
     */
    bool left(Point& a, Point& b);

    /**
     *	@brief Checks the position of the point respect to other two points (a, b).
     */
    bool leftAbove(Point& a, Point& b);

    /**
     *	@brief Assignment operator (override).
     */
    Point& operator=(const Point& point);

    /**
     *	@brief Substraction operator (override).
     */
    virtual Point& operator-(const Point& point);

    /**
     *	@brief Substraction operator (override).
     */
    virtual Point& operator+(const Point& point);

    /**
     *	@brief Substraction operator (override).
     */
    virtual Point& operator*(double scalar);

    /**
     *	@brief Overriding cout call.
     */
    friend std::ostream& operator<<(std::ostream& os, const Point& point);

    /**
     *	@brief Overriding cin call.
     */
    friend std::istream& operator>>(std::istream& is, Point& point);

    /**
     *	@brief Checks the position of the point respect to other two points (a, b).
     */
    bool rightAbove(Point& a, Point& b);

    /**
     *	@brief Checks the position of the point respect to other two points (a, b).
     */
    bool right(Point& a, Point& b);

    /**
     *	@brief Modifies the coordinate values.
     */
    void set(double x, double y);

    /**
     *	@brief Modifies the X coordinate.
     */
    void setX(double x);

    /**
     *	@brief Modifies the Y coordinate.
     */
    void setY(double y);

    /**
     *	@brief Returns the slope between this point and p.
     */
    double slope(Point& p);

    /**
     *	@brief Calculates the double area of the triangle formed by (this, a, b).
     */
    double triangleArea2(Point& a, Point& b);

    /**
     *	@brief Checks the position of the point respect to other two points (a, b).
     */
    Point scalarMult(double t);
};