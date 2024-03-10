#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <cmath>
#include <random>

typedef std::minstd_rand                      RandomNumberGenerator;
typedef std::uniform_real_distribution<float> DoubleUniformDistribution;

/**
 *	@brief Set of utilities to retrieve random values.
 */
namespace RandomUtilities
{
/**
 *	@return New random value retrieved from a random uniform distribution.
 */
float getUniformRandom();

/**
 *	@return New random value retrieved from a random uniform distribution. Note that this value is not in [0, 1].
 */
float getUniformRandom(float min, float max);

/**
 *	@return Random of length up to distanceSquared.
 */
glm::vec3 getRandomToSphere(float radius, float distanceSquared);

/**
 *	@brief Generates a random color in [0, 1] by using getUniformRandom function for each channel.
 */
glm::vec3 getUniformRandomColor();

/**
 *	@brief Generates a random color by using getUniformRandom using Euclidean Distance.
 *         https://en.wikipedia.org/wiki/Color_difference
 */
glm::vec3 getUniformRandomColorEuclideanDistance();

/**
 *	@brief Generates a random color by using getUniformRandom function for each channel.
 */
glm::vec3 getUniformRandomColor(float min, float max);

/**
 *	@return Random hemisphere vector aligned to Z axis.
 */
glm::vec3 getUniformRandomCosineDirection();

/**
 *	@return Random point in unit sphere.
 */
glm::vec3 getUniformRandomInHemisphere(const glm::vec3& normal);

/**
 *	@return Random single integer value.
 */
int getUniformRandomInt(int min, int max);

/**
 *	@return Random point in unit disk.
 */
glm::vec3 getUniformRandomInUnitDisk();

/**
 *	@return Random point in unit sphere.
 */
glm::vec3 getUniformRandomInUnitSphere();

/**
 *	@return Random point in unit dick circumference.
 */
glm::vec3 getUniformRandomInUnitDiskCircumference();

/**
 *	@return Random point in unit square.
 */
glm::vec3 getUniformRandomInUnitSquare();

/**
 *	@return Random point in unit square perimeter.
 */
glm::vec3 getUniformRandomInUnitSquarePerimeter();
}    // namespace RandomUtilities

inline float RandomUtilities::getUniformRandom()
{
    static DoubleUniformDistribution distribution(0.0f, 1.0f);
    // static RandomNumberGenerator     generator(static_cast<unsigned>(time(nullptr)));
    static RandomNumberGenerator generator;
    generator.seed(std::random_device()());
    return distribution(generator);
}

inline float RandomUtilities::getUniformRandom(const float min, const float max)
{
    return min + (max - min) * getUniformRandom();
}

inline glm::vec3 RandomUtilities::getRandomToSphere(const float radius, const float distanceSquared)
{
    const float r1  = getUniformRandom();
    const float r2  = getUniformRandom();
    const float z   = 1 + r2 * (sqrt(1.0f - radius * radius / distanceSquared) - 1.0f);
    const float phi = 2.0f * glm::pi<float>() * r1;
    const float x   = std::cos(phi) * sqrt(1 - z * z);
    const float y   = std::sin(phi) * sqrt(1 - z * z);

    return {x, y, z};
}

inline glm::vec3 RandomUtilities::getUniformRandomColor()
{
    return {getUniformRandom(), getUniformRandom(), getUniformRandom()};
}

inline glm::vec3 RandomUtilities::getUniformRandomColorEuclideanDistance()
{
    glm::vec3           color;
    constexpr glm::vec3 background = glm::vec3(.6f);
    while(true)
    {
        color                 = getUniformRandomColor();
        const double distance = std::sqrt(std::pow(color.x - background.x, 2) + std::pow(color.y - background.y, 2) + std::pow(color.z - background.z, 2));
        if(distance > 0.6)
            return color;
    }
}

inline glm::vec3 RandomUtilities::getUniformRandomColor(const float min, const float max)
{
    return {getUniformRandom(min, max), getUniformRandom(min, max), getUniformRandom(min, max)};
}

inline glm::vec3 RandomUtilities::getUniformRandomCosineDirection()
{
    const float r1 = getUniformRandom(), r2 = getUniformRandom();
    const float z   = sqrt(1 - r2);
    const float phi = 2.0f * glm::pi<float>() * r1;
    const float x   = std::cos(phi) * sqrt(r2);
    const float y   = std::sin(phi) * sqrt(r2);

    return {x, y, z};
}

inline glm::vec3 RandomUtilities::getUniformRandomInHemisphere(const glm::vec3& normal)
{
    const glm::vec3 unitSphere = getUniformRandomInUnitSphere();
    return unitSphere * -1.0f * ((dot(unitSphere, normal) > .0f) * 2.0f - 1.0f);
}

inline int RandomUtilities::getUniformRandomInt(const int min, const int max)
{
    return static_cast<int>(getUniformRandom(static_cast<float>(min), static_cast<float>(max)));
}

inline glm::vec3 RandomUtilities::getUniformRandomInUnitDisk()
{
    while(true)
    {
        glm::vec3 point = glm::vec3(getUniformRandom(-1.0f, 1.0f), getUniformRandom(-1.0f, 1.0f), .0f);
        if(length2(point) >= 1)
            continue;

        return point;
    }
}

inline glm::vec3 RandomUtilities::getUniformRandomInUnitSphere()
{
    glm::vec3 point;
    while(true)
    {
        point = glm::vec3(getUniformRandom(-1.0f, 1.0f), getUniformRandom(-1.0f, 1.0f), getUniformRandom(-1.0f, 1.0f));
        if(length2(point) >= 1)
            continue;

        return point;
    }
}

inline glm::vec3 RandomUtilities::getUniformRandomInUnitDiskCircumference()
{
    glm::vec3 point;
    while(true)
    {
        const float angle = getUniformRandom(0.0f, 2.0f * glm::pi<float>());
        point             = glm::vec3(std::cos(angle), std::sin(angle), .0f);

        if(length2(point) >= 1)
            continue;

        return point;
    }
}

inline glm::vec3 RandomUtilities::getUniformRandomInUnitSquare()
{
    glm::vec3 point;
    while(true)
    {
        point = glm::vec3(getUniformRandom(-1.0f, 1.0f), getUniformRandom(-1.0f, 1.0f), .0f);

        if(point.x >= -1.0f && point.x <= 1.0f && point.y >= -1.0f && point.y <= 1.0f)
            return point;
    }
}

inline glm::vec3 RandomUtilities::getUniformRandomInUnitSquarePerimeter()
{
    glm::vec3 point;
    while(true)
    {
        point = glm::vec3(getUniformRandom(-1.0f, 1.0f), getUniformRandom(-1.0f, 1.0f), .0f);

        if(point.x <= -0.998f || point.x >= 0.998f || point.y <= -0.998f || point.y >= 0.998f)
            return point;
    }
}
