#pragma once

#include "StdAfx.h"

typedef std::mt19937                          RandomNumberGenerator;
typedef std::uniform_real_distribution<float> DoubleUniformDistribution;

/**
 *	@brief Set of utilities to retrieve random values.
 */
namespace RandomUtilities
{
/**
 *	@return Random of length up to distanceSquared.
 */
vec3 getRandomToSphere(float radius, float distanceSquared);

/**
 *	@return New random value retrieved from a random uniform distribution.
 */
float getUniformRandom();

/**
 *	@return New random value retrieved from a random uniform distribution. Note that this value is not in [0, 1].
 */
float getUniformRandom(float min, float max);

/**
 *	@brief Generates a random color in [0, 1] by using getUniformRandom function for each channel.
 */
vec3 getUniformRandomColor();

/**
 *	@brief Generates a random color by using getUniformRandom using Euclidean Distance.
 *         https://en.wikipedia.org/wiki/Color_difference
 */
vec3 getUniformRandomColorEuclideanDistance();

/**
 *	@brief Generates a random color by using getUniformRandom function for each channel.
 */
vec3 getUniformRandomColor(float min, float max);

/**
 *	@return Random hemisphere vector aligned to Z axis.
 */
vec3 getUniformRandomCosineDirection();

/**
 *	@return Random point in unit sphere.
 */
vec3 getUniformRandomInHemisphere(const vec3& normal);

/**
 *	@return Random single integer value.
 */
int getUniformRandomInt(int min, int max);

/**
 *	@return Random point in unit disk.
 */
vec3 getUniformRandomInUnitDisk();

/**
 *	@return Random point in unit sphere.
 */
vec3 getUniformRandomInUnitSphere();

/**
 *	@return Random point in unit dick circumference.
 */
vec3 getUniformRandomInUnitDiskCircumference();

/**
 *	@return Random point in unit square.
 */
vec3 getUniformRandomInUnitSquare();

/**
 *	@return Random point in unit square perimeter.
 */
vec3 getUniformRandomInUnitSquarePerimeter();
}    // namespace RandomUtilities

inline vec3 RandomUtilities::getRandomToSphere(float radius, float distanceSquared)
{
    const float r1  = getUniformRandom();
    const float r2  = getUniformRandom();
    const float z   = 1 + r2 * (sqrt(1.0f - radius * radius / distanceSquared) - 1.0f);
    const float phi = 2.0f * glm::pi<float>() * r1;
    const float x   = std::cos(phi) * sqrt(1 - z * z);
    const float y   = std::sin(phi) * sqrt(1 - z * z);

    return vec3(x, y, z);
}

inline float RandomUtilities::getUniformRandom()
{
    static RandomNumberGenerator     generator;
    generator.seed(std::random_device()());
    static DoubleUniformDistribution distribution(.0f, 1.0f);
    return distribution(generator);
}

inline float RandomUtilities::getUniformRandom(float min, float max)
{
    return min + (max - min) * getUniformRandom();
}

inline vec3 RandomUtilities::getUniformRandomColor()
{
    return {getUniformRandom(), getUniformRandom(), getUniformRandom()};
}

inline vec3 RandomUtilities::getUniformRandomColorEuclideanDistance()
{
    vec3           color;
    constexpr vec3 background = vec3(.6f);
    while(true)
    {
        color                 = getUniformRandomColor();
        const double distance = std::sqrt(std::pow(color.x - background.x, 2) + std::pow(color.y - background.y, 2) + std::pow(color.z - background.z, 2));
        if(distance > 0.6)
            return color;
    }
}

inline vec3 RandomUtilities::getUniformRandomColor(float min, float max)
{
    return {getUniformRandom(min, max), getUniformRandom(min, max), getUniformRandom(min, max)};
}

inline vec3 RandomUtilities::getUniformRandomCosineDirection()
{
    const float r1 = getUniformRandom(), r2 = getUniformRandom();
    const float z   = sqrt(1 - r2);
    const float phi = 2.0f * glm::pi<float>() * r1;
    const float x   = std::cos(phi) * sqrt(r2);
    const float y   = std::sin(phi) * sqrt(r2);

    return {x, y, z};
}

inline vec3 RandomUtilities::getUniformRandomInHemisphere(const vec3& normal)
{
    const vec3 unitSphere = getUniformRandomInUnitSphere();
    return unitSphere * -1.0f * ((dot(unitSphere, normal) > .0f) * 2.0f - 1.0f);
}

inline int RandomUtilities::getUniformRandomInt(int min, int max)
{
    return static_cast<int>(getUniformRandom(min, max));
}

inline vec3 RandomUtilities::getUniformRandomInUnitDisk()
{
    while(true)
    {
        vec3 point = vec3(getUniformRandom(-1.0f, 1.0f), getUniformRandom(-1.0f, 1.0f), .0f);
        if(length2(point) >= 1)
            continue;

        return point;
    }
}

inline vec3 RandomUtilities::getUniformRandomInUnitSphere()
{
    vec3 point;
    while(true)
    {
        point = vec3(getUniformRandom(-1.0f, 1.0f), getUniformRandom(-1.0f, 1.0f), getUniformRandom(-1.0f, 1.0f));
        if(length2(point) >= 1)
            continue;

        return point;
    }
}

inline vec3 RandomUtilities::getUniformRandomInUnitDiskCircumference()
{
    vec3 point;
    while(true)
    {
        const float angle = getUniformRandom(0.0f, 2.0f * glm::pi<float>());
        point             = vec3(std::cos(angle), std::sin(angle), .0f);

        if(length2(point) >= 1)
            continue;

        return point;
    }
}

inline vec3 RandomUtilities::getUniformRandomInUnitSquare()
{
    vec3 point;
    while(true)
    {
        point = vec3(getUniformRandom(-1.0f, 1.0f), getUniformRandom(-1.0f, 1.0f), .0f);

        if(point.x >= -1.0f && point.x <= 1.0f && point.y >= -1.0f && point.y <= 1.0f)
            return point;
    }
}

inline vec3 RandomUtilities::getUniformRandomInUnitSquarePerimeter()
{
    vec3 point;
    while(true)
    {
        point = vec3(getUniformRandom(-1.0f, 1.0f), getUniformRandom(-1.0f, 1.0f), .0f);

        if(point.x <= -0.998f || point.x >= 0.998f || point.y <= -0.998f || point.y >= 0.998f)
            return point;
    }
}
