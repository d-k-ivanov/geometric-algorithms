#pragma once

#include "Vect3d.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include <iostream>

namespace GDSA::Geometry
{
class AABB
{
protected:
    Vect3d _min, _max;

public:
    /**
     *	@brief Constructor.
     */
    AABB(const Vect3d& min = Vect3d(HUGE_VAL), const Vect3d& max = Vect3d(-HUGE_VAL));

    /**
     *	@brief Copy constructor,
     */
    AABB(const AABB& aabb);

    /**
     *	@brief Destructor.
     */
    virtual ~AABB();

    /**
     *	@brief Returns the size of the cube.
     */
    Vect3d getSize() const;

    /**
     *	@brief Returns the central point of the cube.
     */
    Vect3d getCenter() const;

    /**
     *	@brief Returns the vector that goes from the center to the maximum point.
     */
    Vect3d getExtent() const;

    /**
     *	@brief Returns the lowest corner of the cube.
     */
    Vect3d getMin();

    /**
     *	@brief Returns the maximum points of the cube.
     */
    Vect3d getMax();

    /**
     *	@brief Modifies the minimum point.
     */
    void setMin(const Vect3d& min);

    /**
     *	@brief Modifies the maximum point.
     */
    void setMax(const Vect3d& max);

    /**
     *	@brief AABB dot product.
     */
    AABB dot(const glm::mat4& matrix) const;

    /**
     *	@brief Assignment operator.
     */
    AABB& operator=(const AABB& orig);

    /**
     *	@brief Updates the cube with a new cube.
     */
    void update(AABB& aabb);

    /**
     *	@brief Updates the cube with a new point.
     */
    void update(const Vect3d& point);

    /**
     *	@brief Shows some information of the vector as debugging info.
     */
    friend std::ostream& operator<<(std::ostream& os, const AABB& aabb);
};
}    // namespace GDSA