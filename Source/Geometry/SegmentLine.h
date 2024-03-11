#pragma once

#include "Point.h"
#include "Vect2d.h"

class Line;
class RayLine;

class SegmentLine
{
    friend class DrawSegment;

protected:
    Point _orig, _dest;

protected:
    /**
     *	@brief Returns the parametric value T0 to calculate the distance between a point and any geometric object like lines, segments or raylines.
     */
    float getDistanceT0(Vect2d& point);

    /**
     *	@brief Obstaints the parameters t and s where both lines intersects, if they do.
     */
    // virtual bool intersects(Vect2d& p1, Vect2d& p2, float& t, float& s);

    /**
     *	@brief Determine the values ​​of s and t in the calculation of the intersection of two lines that they contain AB (this) and CD.
     */
    virtual bool intersects(Vect2d& c, Vect2d& d, double& s, double& t);

    /**
     *	@brief Segment-Line Intersection (this:(A,B), r(C,D)). Intersection exists if 0<=s<=1
     */
    virtual bool intersects(Line& r, Vect2d& res);

    /**
     *	@brief Segment-Ray Intersection (this:(A,B), r(C,D)). Intersection exists if 0<=s<=1 and 0<=t
     */
    virtual bool intersects(RayLine& r, Vect2d& res);

    /**
     *	@brief Segment-Segment Intersection (this:(A,B), r(C,D)). There is an intersection if 0<=s<=1 and 0<=t<=1
     */
    virtual bool intersects(SegmentLine& r, Vect2d& res);

public:
    /**
     *	@brief Default constructor.
     */
    SegmentLine();

    /**
     *	@brief Constructor.
     */
    SegmentLine(const Point& a, const Point& b);

    /**
     *	@brief Copy constructor.
     */
    SegmentLine(const SegmentLine& segment);

    /**
     *	@brief Constructor.
     */
    SegmentLine(double ax, double ay, double bx, double by);

    /**
     *	@brief Destructor.
     */
    virtual ~SegmentLine();

    /**
     *	@brief Returns the origin of the segment.
     */
    Point getA();

    /**
     *	@brief Returns the end of the segment.
     */
    Point getB();

    /**
     *	@brief Returns the constant of the equation of the implied line: c = y-mx.
     */
    double getC();

    /**
     *	@brief Checks if a segment is different to this one.
     */
    bool distinct(SegmentLine& segment);

    /**
     *	@brief Distance from a point defined by 'vector' to this segment.
     */
    double distPointSegment(Vect2d& vector);

    /**
     *	@brief Checks if a segment is equal to this one.
     */
    bool equal(SegmentLine& segment);

    /**
     *	@brief It obtains the point belonging to the segment or colineal to it for a concrete t in the parametric equation: result = a + t (b-a).
     */
    Point getPoint(double t);

    /**
     *	@brief Determines whether two segments intersect improperly, that is, when one end of a segment is contained in the other. Use integer arithmetic.
     */
    bool impSegmentIntersection(SegmentLine& segment);

    /**
     *	@brief Determines whether a segment is horizontal or not (using EPSILON).
     */
    bool isHorizontal();

    /**
     *	@brief Determines whether or not a segment is vertical (using EPSILON).
     */
    bool isVertical();

    /**
     *	@brief Check if the parameter t is valid to get a point of the segment.
     */
    virtual bool isTvalid(double t);

    /**
     *	@brief Determines whether p is in the left of SegmentLine.
     */
    bool left(Point& p);

    /**
     *	@brief Returns the length of the segment.
     */
    double length();

    /**
     *	@brief Assignment operator.
     */
    virtual SegmentLine& operator=(const SegmentLine& segment);

    /**
     *	@brief Overriding cout call.
     */
    friend std::ostream& operator<<(std::ostream& os, const SegmentLine& segment);

    /**
     *	@brief Determines whether two segments intersect in their own way, that is, when they intersect completely. Use only arithmetic.
     */
    virtual bool segmentIntersection(SegmentLine& l);

    /**
     *	@brief Modifies the origin of the segment.
     */
    void setA(Point& p);

    /**
     *	@brief Returns the slope of the implied straight line equation: m = (yb-ya) / (xb-xa).
     */
    double slope();

    /**
     *	@brief Returns the area formed by the triangle composed of the current SegmentLine and the union of its bounds with p.
     */
    double triangleArea2(Point& p);
};
