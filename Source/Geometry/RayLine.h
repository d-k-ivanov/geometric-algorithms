#pragma once

#include "SegmentLine.h"

class RayLine : public SegmentLine
{
    friend Line;

public:
    /**
     *	@brief Constructor of a rayline given its two points.
     */
    RayLine(const Point& a, const Point& b);

    /**
     *	@brief Copy constructor.
     */
    RayLine(const RayLine& s);

    /**
     *	@brief Destructor.
     */
    ~RayLine();

    /**
     *	@brief Checks if this rayline intersects with a segment in an incorrect way.
     */
    bool incorrectSegmentIntersection(SegmentLine& segment);

    /**
     *	@brief Checks if a rayline intersects with a line.
     *	@param line
     *	@param intersection If rayline and line intersect, then this point is the intersection. Otherwise this point is not valid.
     */
    bool intersects(Line& line, Vect2d& intersection) override;

    /**
     *	@brief Checks if a rayline intersects with this one.
     *	@param ray
     *	@param intersection If both raylines intersect, then this point is the intersection. Otherwise this point is not valid.
     */
    bool intersects(RayLine& ray, Vect2d& intersection) override;

    /**
     *	@brief Checks if a segment intersects with a segment.
     *	@param segment
     *	@param intersection If rayline and segment intersect, then this point is the intersection. Otherwise this point is not valid.
     */
    bool intersects(SegmentLine& segment, Vect2d& intersection) override;

    /**
     *	@brief Distance from a point defined by 'vector' to this rayline.
     */
    double distanceToPoint(Vect2d& vector);

    /**
     *	@brief Checks if the parameter t is valid to get a point of the rayline.
     */
    virtual bool isTvalid(double t) { return t >= 0; }

    /**
     *	@brief Assignment operator.
     */
   RayLine& operator=(const RayLine& rayline);

    /**
     *	@brief Overriding cout call.
     */
    friend std::ostream& operator<<(std::ostream& os, const RayLine& ray);
};
