#pragma once

#include "Vect3d.h"

namespace GDSA::Geometry
{
/**
 *  @brief Represents any line of the geometry module.
 */
class Edge3d
{
protected:
    Vect3d _orig;
    Vect3d _dest;
    Vect3d _direction;

protected:
    /**
     *  @brief Checks if the parametric t values is valid for our subclass.
     */
    virtual bool isTvalid(double t) = 0;

public:
    Edge3d();
    Edge3d(const Vect3d& orig, const Vect3d& dest);
    Edge3d(const Edge3d& edge);

    virtual ~Edge3d() = default;

    Edge3d& operator=(const Edge3d& edge) = default;

    /**
     *  @brief Returns the destination point.
     */
    Vect3d& getDestination();

    /**
     *  @brief Returns the initial point.
     */
    Vect3d& getOrigin();

    /**
     *	@return Edge direction.
     */
    Vect3d getDirection() const;

    /**
     *  @brief Returns a point given the parametric value. It may be wrong if the t value is not valid for the specific subclass.
     */
    virtual Vect3d getPoint(double t);

    /**
     *  @brief Returns a vector with all the coordinate values of the segment (origin and dest).
     */
    std::vector<double> getVertices() const;

    /**
     *	@brief Modifies the final point.
     */
    void setDest(const Vect3d& dest);

    /**
     *	@brief Modifies the edge direction.
     */
    void setDirection(const Vect3d& direction);

    /**
     *	@brief Modifies the edge starting point.
     */
    void setOrigin(const Vect3d& orig);

    /**
     *  @brief Overriding cout call.
     */
    friend std::ostream& operator<<(std::ostream& os, const Edge3d& edge);
};
}    // namespace GDSA::Geometry
