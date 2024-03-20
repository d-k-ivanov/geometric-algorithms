#include "AABB.h"

namespace GDSA::Geometry
{
AABB::AABB(const Vect3d& min, const Vect3d& max)
    : _min(min)
    , _max(max)
{
}

AABB::AABB(const AABB& aabb)
    : _min(aabb._min)
    , _max(aabb._max)
{
}

AABB::~AABB()
{
}

Vect3d AABB::getSize() const
{
    return _max.sub(_min);
}

Vect3d AABB::getCenter() const
{
    return _max.add(_min) / 2.0f;
}

Vect3d AABB::getExtent() const
{
    return _max.sub(getCenter());
}

Vect3d AABB::getMin()
{
    return _min;
}

Vect3d AABB::getMax()
{
    return _max;
}

void AABB::setMin(const Vect3d& min)
{
    _min = min;
}

void AABB::setMax(const Vect3d& max)
{
    _max = max;
}

AABB AABB::dot(const glm::mat4& matrix) const
{
    const auto minMX = matrix * glm::vec4(_min.toGlmVec3(), 1.0f);
    const auto maxMX = matrix * glm::vec4(_max.toGlmVec3(), 1.0f);

    const Vect3d min = {minMX.x, minMX.y, minMX.z};
    const Vect3d max = {maxMX.x, maxMX.y, maxMX.z};

    return {min, max};
}

AABB& AABB::operator=(const AABB& orig)
{
    _min = orig._min;
    _max = orig._max;

    return *this;
}

void AABB::update(AABB& aabb)
{
    this->update(aabb.getMax());
    this->update(aabb.getMin());
}

void AABB::update(const Vect3d& point)
{
    if(point._x < _min._x)
    {
        _min._x = point._x;
    }
    if(point._y < _min._y)
    {
        _min._y = point._y;
    }
    if(point._z < _min._z)
    {
        _min._z = point._z;
    }

    if(point._x > _max._x)
    {
        _max._x = point._x;
    }
    if(point._y > _max._y)
    {
        _max._y = point._y;
    }
    if(point._z > _max._z)
    {
        _max._z = point._z;
    }
}

std::ostream& operator<<(std::ostream& os, const AABB& aabb)
{
    os << "Maximum: " << aabb._max << ", minimum: " << aabb._min;
    return os;
}
}    // namespace GDSA::Geometry