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
    Vect3d _orig, _dest;
    Vect3d _direction;

protected:
    /**
     *  @brief Checks if the parametric t values is valid for our subclass.
     */
    virtual bool isTvalid(double t) = 0;

public:
    /**
     *  @brief Default constructor.
     */
    Edge3d();

    /**
     *  @brief Constructor.
     */
    Edge3d(Vect3d& orig, Vect3d& dest);

    /**
     *  @brief Copy constructor.
     */
    Edge3d(const Edge3d& edge);

    /**
     *  @brief Destructor.
     */
    virtual ~Edge3d();

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
     *  @brief Assignment operator.
     */
    Edge3d& operator=(const Edge3d& edge);

    /**
     *  @brief Overriding cout call.
     */
    friend std::ostream& operator<<(std::ostream& os, const Edge3d& edge);
};
}    // namespace GDSA::Geometry
