#pragma once

#include "Point.h"
#include "Polygon.h"

enum CircleCircleRelations
{
    CONCENTRIC,
    EXTERNAL,
    INTERNAL,
    SECANT,
    INTERIOR_TANG,
    EXTERNAL_TANG,
    NO_RELATION
};

enum CircleLineRelations
{
    INTERSECT,
    TANGENT,
    NO_INTERSECT
};

class Circle
{
protected:
    Point  _center;
    double _radius;

public:
    /**
     *  @brief Default constructor. Coordinates are initialized with an invalid value.
     */
    Circle()
        : _center(Point(0, 0))
        , _radius(1.0)
    {
    }

    /**
     *  @brief Constructor.
     */
    Circle(const Point& center, const double radius)
        : _center(center)
        , _radius(radius)
    {
    }

    /**
     *  @brief Copy constructor.
     */
    Circle(const Circle& orig) = default;

    /**
     *  @brief Destructor.
     */
    virtual ~Circle() = default;

    /**
     *   @brief Checks if the point is inside the circle.
     *   @param point
     *   @return true if p is inside the circle
     */
    bool isInside(Point& point);

    /**
     *   @return A polygon with circular shape.
     */
    Polygon getPointsCircle(uint32_t numSubdivisions = 256) const;

    /**
     *   @return Central point.
     */
    Point getCenter() const { return _center; }

    /**
     *   @return Circle radius.
     */
    double getRadius() const { return _radius; }

    /**
     *   @return Relation of a Circle to this circle.
     */
    CircleCircleRelations relation(const Circle& other) const;

    /**
     *   @return Relations of a Line, Ray or Segment to this circle.
     */
    CircleLineRelations relation(Line& line) const;
    CircleLineRelations intersect(Line& line, Point& p1, Point& p2) const;
    CircleLineRelations intersect(RayLine& rayLine, Point& p1, Point& p2) const;
    CircleLineRelations intersect(SegmentLine& segment, Point& p1, Point& p2);

    /**
     *  @brief Assignment operator (override).
     */
    Circle& operator=(const Circle& circle);
};
