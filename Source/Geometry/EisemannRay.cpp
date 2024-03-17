#include "EisemannRay.h"

#include "Geometry/BasicGeometry.h"

const std::vector<uint16_t> EisemannRay::_xValue {2, 4, 8};
const std::vector<uint16_t> EisemannRay::_yValue {16, 32, 64};
const std::vector<uint16_t> EisemannRay::_zValue {128, 256, 512};

EisemannRay::TestFunctionMap EisemannRay::_testFunction = std::move(EisemannRay::initializeTestFunctions());

EisemannRay::EisemannRay(const Ray3d& ray)
    : Ray3d(ray)
{
    this->computeParameters();
}

EisemannRay::EisemannRay(const EisemannRay& ray)
    : Ray3d(ray)
{
    this->computeParameters();
}

EisemannRay EisemannRay::operator=(const EisemannRay& ray)
{
    if(this != &ray)
    {
        Ray3d::operator=(ray);
        this->computeParameters();
    }

    return *this;
}

bool EisemannRay::intersect(AABB& aabb) const
{
    const auto function = _testFunction[_rayType];
    return function(*this, aabb.getMin().toGlmVec3(), aabb.getMax().toGlmVec3());
}

void EisemannRay::classifyRay()
{
    Vect3d dir = _direction;
    if(BasicGeometry::equal(dir.getX(), 0.0f))
        dir._x = 0.0f;

    if(BasicGeometry::equal(dir.getY(), 0.0f))
        dir._y = 0.0f;

    if(BasicGeometry::equal(dir.getZ(), 0.0f))
        dir._z = 0.0f;

    const int i = BasicGeometry::sign(dir.getX()) + 1;
    const int j = BasicGeometry::sign(dir.getY()) + 1;
    const int k = BasicGeometry::sign(dir.getZ()) + 1;

    // b00110100 === MPO
    //    ||||||_ k non-zero (false)
    //    |||||_ k negative (false)
    //    ||||_ j non-zero (true)
    //    |||_ j negative (false)
    //    ||_ i non-zero (true)
    //    |_ i negative (true)
    const int rayType = _xValue[i] + _yValue[j] + _zValue[k];
    _rayType          = static_cast<RayType>(rayType);
}

void EisemannRay::computeParameters()
{
    // Ray direction
    const double i = _direction.getX();
    const double j = _direction.getY();
    const double k = _direction.getZ();

    // Ray origin
    const double x = _orig.getX();
    const double y = _orig.getY();
    const double z = _orig.getZ();

    const double origLength = glm::length(_orig.toGlmVec3());
    const double dirLength  = glm::length(_direction.toGlmVec3());

    if(BasicGeometry::equal(origLength, 0.0f) && BasicGeometry::equal(dirLength, 0.0f))
    {
        return;
    }

    const double ii = _ii = BasicGeometry::equal(i, 0.0f) ? 0.0f : 1.0f / i;
    const double ij = _ij = BasicGeometry::equal(j, 0.0f) ? 0.0f : 1.0f / j;
    const double ik = _ik = BasicGeometry::equal(k, 0.0f) ? 0.0f : 1.0f / k;

    // Ray slope
    const double ibyj = _ibyj = i * ij;
    const double jbyi = _jbyi = j * ii;
    const double jbyk = _jbyk = j * ik;
    const double kbyj = _kbyj = k * ij;
    const double ibyk = _ibyk = i * ik;
    const double kbyi = _kbyi = k * ii;
    _cxy                      = y - jbyi * x;
    _cxz                      = z - kbyi * x;
    _cyx                      = x - ibyj * y;
    _cyz                      = z - kbyj * y;
    _czx                      = x - ibyk * z;
    _czy                      = y - jbyk * z;

    this->classifyRay();
}

EisemannRay::TestFunctionMap EisemannRay::initializeTestFunctions()
{
    TestFunctionMap map;

    map[MMM] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[0] < lb[0] || ro[1] < lb[1] || ro[2] < lb[2] || ray._jbyi * lb[0] - ub[1] + ray._cxy > 0 || ray._ibyj * lb[1] - ub[0] + ray._cyx > 0 || ray._jbyk * lb[2] - ub[1] + ray._czy > 0 || ray._kbyj * lb[1] - ub[2] + ray._cyz > 0 || ray._kbyi * lb[0] - ub[2] + ray._cxz > 0 || ray._ibyk * lb[2] - ub[0] + ray._czx > 0);
    };

    map[MMP] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[0] < lb[0] || ro[1] < lb[1] || ro[2] > ub[2] || ray._jbyi * lb[0] - ub[1] + ray._cxy > 0 || ray._ibyj * lb[1] - ub[0] + ray._cyx > 0 || ray._jbyk * ub[2] - ub[1] + ray._czy > 0 || ray._kbyj * lb[1] - lb[2] + ray._cyz < 0 || ray._kbyi * lb[0] - lb[2] + ray._cxz < 0 || ray._ibyk * ub[2] - ub[0] + ray._czx > 0);
    };

    map[MPM] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[0] < lb[0] || ro[1] > ub[1] || ro[2] < lb[2] || ray._jbyi * lb[0] - lb[1] + ray._cxy < 0 || ray._ibyj * ub[1] - ub[0] + ray._cyx > 0 || ray._jbyk * lb[2] - lb[1] + ray._czy < 0 || ray._kbyj * ub[1] - ub[2] + ray._cyz > 0 || ray._kbyi * lb[0] - ub[2] + ray._cxz > 0 || ray._ibyk * lb[2] - ub[0] + ray._czx > 0);
    };

    map[MPP] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[0] < lb[0] || ro[1] > ub[1] || ro[2] > ub[2] || ray._jbyi * lb[0] - lb[1] + ray._cxy < 0 || ray._ibyj * ub[1] - ub[0] + ray._cyx > 0 || ray._jbyk * ub[2] - lb[1] + ray._czy < 0 || ray._kbyj * ub[1] - lb[2] + ray._cyz < 0 || ray._kbyi * lb[0] - lb[2] + ray._cxz < 0 || ray._ibyk * ub[2] - ub[0] + ray._czx > 0);
    };

    map[PMM] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[0] > ub[0] || ro[1] < lb[1] || ro[2] < lb[2] || ray._jbyi * ub[0] - ub[1] + ray._cxy > 0 || ray._ibyj * lb[1] - lb[0] + ray._cyx < 0 || ray._jbyk * lb[2] - ub[1] + ray._czy > 0 || ray._kbyj * lb[1] - ub[2] + ray._cyz > 0 || ray._kbyi * ub[0] - ub[2] + ray._cxz > 0 || ray._ibyk * lb[2] - lb[0] + ray._czx < 0);
    };

    map[PMP] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[0] > ub[0] || ro[1] < lb[1] || ro[2] > ub[2] || ray._jbyi * ub[0] - ub[1] + ray._cxy > 0 || ray._ibyj * lb[1] - lb[0] + ray._cyx < 0 || ray._jbyk * ub[2] - ub[1] + ray._czy > 0 || ray._kbyj * lb[1] - lb[2] + ray._cyz < 0 || ray._kbyi * ub[0] - lb[2] + ray._cxz < 0 || ray._ibyk * ub[2] - lb[0] + ray._czx < 0);
    };

    map[PPM] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[0] > ub[0] || ro[1] > ub[1] || ro[2] < lb[2] || ray._jbyi * ub[0] - lb[1] + ray._cxy < 0 || ray._ibyj * ub[1] - lb[0] + ray._cyx < 0 || ray._jbyk * lb[2] - lb[1] + ray._czy < 0 || ray._kbyj * ub[1] - ub[2] + ray._cyz > 0 || ray._kbyi * ub[0] - ub[2] + ray._cxz > 0 || ray._ibyk * lb[2] - lb[0] + ray._czx < 0);
    };

    map[PPP] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[0] > ub[0] || ro[1] > ub[1] || ro[2] > ub[2] || ray._jbyi * ub[0] - lb[1] + ray._cxy < 0 || ray._ibyj * ub[1] - lb[0] + ray._cyx < 0 || ray._jbyk * ub[2] - lb[1] + ray._czy < 0 || ray._kbyj * ub[1] - lb[2] + ray._cyz < 0 || ray._kbyi * ub[0] - lb[2] + ray._cxz < 0 || ray._ibyk * ub[2] - lb[0] + ray._czx < 0);
    };

    map[POO] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro  = ray._orig.toGlmVec3();
        const double    ro1 = ro[1];
        const double    ro2 = ro[2];

        return !(ro[0] > ub[0] || ro1 < lb[1] || ro1 > ub[1] || ro2 < lb[2] || ro2 > ub[2]);
    };

    map[MOO] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[0] < lb[0] || ro[1] < lb[1] || ro[1] > ub[1] || ro[2] < lb[2] || ro[2] > ub[2]);
    };

    map[OPO] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[1] > ub[1] || ro[0] < lb[0] || ro[0] > ub[0] || ro[2] < lb[2] || ro[2] > ub[2]);
    };

    map[OMO] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[1] < lb[1] || ro[0] < lb[0] || ro[0] > ub[0] || ro[2] < lb[2] || ro[2] > ub[2]);
    };

    map[OOP] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[2] > ub[2] || ro[0] < lb[0] || ro[0] > ub[0] || ro[1] < lb[1] || ro[1] > ub[1]);
    };

    map[OOM] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[2] < lb[2] || ro[0] < lb[0] || ro[0] > ub[0] || ro[1] < lb[1] || ro[1] > ub[1]);
    };

    map[OMM] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[0] < lb[0] || ro[0] > ub[0] || ro[1] < lb[1] || ro[2] < lb[2] || ray._jbyk * lb[2] - ub[1] + ray._czy > 0 || ray._kbyj * lb[1] - ub[2] + ray._cyz > 0);
    };

    map[OMP] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[0] < lb[0] || ro[0] > ub[0] || ro[1] < lb[1] || ro[2] > ub[2] || ray._jbyk * ub[2] - ub[1] + ray._czy > 0 || ray._kbyj * lb[1] - lb[2] + ray._cyz < 0);
    };

    map[OPM] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[0] < lb[0] || ro[0] > ub[0] || ro[1] > ub[1] || ro[2] < lb[2] || ray._jbyk * lb[2] - lb[1] + ray._czy < 0 || ray._kbyj * ub[1] - ub[2] + ray._cyz > 0);
    };

    map[OPP] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[0] < lb[0] || ro[0] > ub[0] || ro[1] > ub[1] || ro[2] > ub[2] || ray._jbyk * ub[2] - lb[1] + ray._czy < 0 || ray._kbyj * ub[1] - lb[2] + ray._cyz < 0);
    };

    map[MOM] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[1] < lb[1] || ro[1] > ub[1] || ro[0] < lb[0] || ro[2] < lb[2] || ray._kbyi * lb[0] - ub[2] + ray._cxz > 0 || ray._ibyk * lb[2] - ub[0] + ray._czx > 0);
    };

    map[MOP] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[1] < lb[1] || ro[1] > ub[1] || ro[0] < lb[0] || ro[2] > ub[2] || ray._kbyi * lb[0] - lb[2] + ray._cxz < 0 || ray._ibyk * ub[2] - ub[0] + ray._czx > 0);
    };

    map[POM] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[1] < lb[1] || ro[1] > ub[1] || ro[0] > ub[0] || ro[2] < lb[2] || ray._kbyi * ub[0] - ub[2] + ray._cxz > 0 || ray._ibyk * lb[2] - lb[0] + ray._czx < 0);
    };

    map[POP] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[1] < lb[1] || ro[1] > ub[1] || ro[0] > ub[0] || ro[2] > ub[2] || ray._kbyi * ub[0] - lb[2] + ray._cxz < 0 || ray._ibyk * ub[2] - lb[0] + ray._czx < 0);
    };

    map[MMO] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[2] < lb[2] || ro[2] > ub[2] || ro[0] < lb[0] || ro[1] < lb[1] || ray._jbyi * lb[0] - ub[1] + ray._cxy > 0 || ray._ibyj * lb[1] - ub[0] + ray._cyx > 0);
    };

    map[MPO] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[2] < lb[2] || ro[2] > ub[2] || ro[0] < lb[0] || ro[1] > ub[1] || ray._jbyi * lb[0] - lb[1] + ray._cxy < 0 || ray._ibyj * ub[1] - ub[0] + ray._cyx > 0);
    };

    map[PMO] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[2] < lb[2] || ro[2] > ub[2] || ro[0] > ub[0] || ro[1] < lb[1] || ray._jbyi * ub[0] - ub[1] + ray._cxy > 0 || ray._ibyj * lb[1] - lb[0] + ray._cyx < 0);
    };

    map[PPO] = [](const EisemannRay& ray, const glm::vec3& lb, const glm::vec3& ub) -> int
    {
        const glm::vec3 ro = ray._orig.toGlmVec3();

        return !(ro[2] < lb[2] || ro[2] > ub[2] || ro[0] > ub[0] || ro[1] > ub[1] || ray._jbyi * ub[0] - lb[1] + ray._cxy < 0 || ray._ibyj * ub[1] - lb[0] + ray._cyx < 0);
    };

    return map;
}
