#pragma once

#include "SegmentLine.h"

namespace GDSA::Geometry
{
class Line : public SegmentLine
{
    friend RayLine;

public:
    Line() = default;
    Line(const Point& a, const Point& b);
    Line(const Line& s) = default;

    ~Line() override = default;

    Line& operator=(const Line& line);

    /**
     *  @brief Checks if the parameter t is valid to obtain a point for a line (it's always valid, we just need to override).
     */
    bool isTvalid(double t) override;

    /**
     *  @brief Checks if the specified line intersects with this one.
     *  @param line
     *  @param intersection If both lines intersect, then this point is the intersection. Otherwise this point is not valid.
     */
    bool intersects(Line& line, Vect2d& intersection) override;

    /**
     *  @brief Checks if the specified rayline intersects with this line.
     *  @param ray
     *  @param intersection If rayline and line intersect, then this point is the intersection. Otherwise this point is not valid.
     */
    bool intersects(RayLine& ray, Vect2d& intersection) override;

    /**
     *  @brief Checks if the specified segment intersects with this line.
     *  @param segment
     *  @param intersection If line and segment intersect, then this point is the intersection. Otherwise this point is not valid.
     */
    bool intersects(SegmentLine& segment, Vect2d& intersection) override;

    /**
     *  @brief Distance from a point defined by 'vector' to this line.
     */
    double distancePointLine(Vect2d& v);

    /**
     *  @brief Checks if a segment intersects with this line (proper intersection).
     */
    bool segmentIntersection(SegmentLine& segment) override;

    /**
     *  @brief Checks if a segment line generates an incorrect intersection.
     */
    bool impSegmentIntersection(SegmentLine& segment) override;

    /**
     *  @brief Overriding cout call.
     */
    friend std::ostream& operator<<(std::ostream& os, const Line& line);
};
}    // namespace GDSA::Geometry