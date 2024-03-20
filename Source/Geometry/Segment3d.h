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
    virtual bool isTvalid(double t) { return (t >= 0.0f && t <= 1.0f); }

public:
    /**
     *  @brief Creates a segment with default point values.
     */
    Segment3d() {};

    /**
     *  @brief Constructor.
     */
    Segment3d(Vect3d& orig, Vect3d& dest);

    /**
     *  @brief Copy constructor.
     */
    Segment3d(const Segment3d& segment);

    /**
     *  @brief Destructor.
     */
    virtual ~Segment3d();

    /**
     *  @brief Assignment operator.
     */
    virtual Segment3d& operator=(const Segment3d& segment);

    /**
     *  @brief Shows the segment data at the debug window.
     */
    friend std::ostream& operator<<(std::ostream& os, const Segment3d& segment);
};
}    // namespace GDSA::Geometry
