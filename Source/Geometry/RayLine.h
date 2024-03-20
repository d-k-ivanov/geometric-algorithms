#pragma once

#include "SegmentLine.h"

namespace GDSA::Geometry
{
class RayLine : public SegmentLine
{
    friend Line;

public:
    RayLine() = default;
    RayLine(const Point& a, const Point& b);
    RayLine(const RayLine& s) = default;

    ~RayLine() override = default;

    RayLine& operator=(const RayLine& rayline);

    /**
     *  @brief Checks if this rayline intersects with a segment in an incorrect way.
     */
    bool incorrectSegmentIntersection(SegmentLine& segment) const;

    /**
     *  @brief Checks if a rayline intersects with a line.
     *  @param line
     *  @param intersection If rayline and line intersect, then this point is the intersection. Otherwise this point is not valid.
     */
    bool intersects(Line& line, Vect2d& intersection) override;

    /**
     *  @brief Checks if a rayline intersects with this one.
     *  @param ray
     *  @param intersection If both raylines intersect, then this point is the intersection. Otherwise this point is not valid.
     */
    bool intersects(RayLine& ray, Vect2d& intersection) override;

    /**
     *  @brief Checks if a segment intersects with a segment.
     *  @param segment
     *  @param intersection If rayline and segment intersect, then this point is the intersection. Otherwise this point is not valid.
     */
    bool intersects(SegmentLine& segment, Vect2d& intersection) override;

    /**
     *  @brief Distance from a point defined by 'vector' to this rayline.
     */
    double distanceToPoint(Vect2d& vector);

    /**
     *  @brief Checks if the parameter t is valid to get a point of the rayline.
     */
    virtual bool isTvalid(double t) { return t >= 0; }

    /**
     *  @brief Overriding cout call.
     */
    friend std::ostream& operator<<(std::ostream& os, const RayLine& ray);
};
}    // namespace GDSA::Geometry
