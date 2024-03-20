#pragma once

#include "Edge3d.h"
#include "Line3d.h"

namespace GDSA::Geometry
{
class Line3d : public Edge3d
{
public:
    enum classifyLines
    {
        NON_INTERSECT,
        PARALLEL,
        INTERSECT,
        THESAME
    };

protected:
    /**
     *  @brief Checks if the parametric value t is valid. Any value is valid for a line.
     */
    bool isTvalid(double t) override;

public:
    Line3d() = default;
    Line3d(Vect3d& orig, Vect3d& dest);
    Line3d(const Line3d& line) = default;
    ~Line3d() override         = default;

    /**
     *  @brief Distance between two lines.
     */
    double distance(Line3d& line);

    /**
     *  @brief Distance between point 'p' and this.
     */
    double distance(const Vect3d& p) const;

    /**
     *  @brief Returns the normal to this line that passes through point 'p'.
     */
    Line3d normalLine(Vect3d& p) const;

    /**
     *  @brief Returns true if the lines are parallel.
     */
    bool isParallel(Line3d& line);

    /**
     *  @brief Returns true if the lines are perpendicular.
     */
    bool isPerpendicular(Line3d& line);

    /**
     *  @brief Assignment operator.
     */
    virtual Line3d& operator=(const Line3d& line);

    /**
     *  @brief Shows line data at the debug window.
     */
    friend std::ostream& operator<<(std::ostream& os, const Line3d& line);
};
}    // namespace GDSA::Geometry
