#pragma once

#include "Vect3d.h"

/**
 *  @brief Represents any line of the geometry module.
 */
class Edge3d
{
protected:
    Vect3d _orig, _dest;

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
     *  @brief Returns a point given the parametric value. It may be wrong if the t value is not valid for the specific subclass.
     */
    virtual Vect3d getPoint(double t);

    /**
     *  @brief Returns a vector with all the coordinate values of the segment (origin and dest).
     */
    std::vector<double> getVertices();

    /**
     *  @brief Assignment operator.
     */
    virtual Edge3d& operator=(const Edge3d& edge);

    /**
     *  @brief Overriding cout call.
     */
    friend std::ostream& operator<<(std::ostream& os, const Edge3d& edge);
};
