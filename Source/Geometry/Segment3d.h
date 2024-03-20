#pragma once

#include "Edge3d.h"

namespace GDSA::Geometry
{
/**
 *  @brief This class represent a segment formed by two points.
 */
class Segment3d : public Edge3d
{
protected:
    /**
     *  @brief Checks if the parametric value t is between 0 and 1 as a segment is not infinite.
     */
    bool isTvalid(double t) override { return (t >= 0.0f && t <= 1.0f); }

public:
    Segment3d() = default;
    Segment3d(const Vect3d& orig, const Vect3d& dest);
    Segment3d(const Segment3d& segment) = default;
    ~Segment3d() override               = default;

    Segment3d& operator=(const Segment3d& segment);

    /**
     *  @brief Shows the segment data at the debug window.
     */
    friend std::ostream& operator<<(std::ostream& os, const Segment3d& segment);
};
}    // namespace GDSA::Geometry
