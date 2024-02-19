#pragma once

#include <chrono>
#include <iostream>

/**
 *	@brief Utilities which help us to measure execution time.
 *	@author Alfonso López Ruiz.
 */
namespace ChronoUtilities
{
//!< Units we can use to return the measured time
enum TimeUnit : int
{
    SECONDS      = 1000000000,
    MILLISECONDS = 1000000,
    MICROSECONDS = 1000,
    NANOSECONDS  = 1
};

namespace
{
    std::chrono::high_resolution_clock::time_point _initTime;
}

/**
 *	@return Measured time in the selected time unit since the clock was started. By default the time unit is milliseconds.
 */
long long getDuration(const TimeUnit timeUnit = MILLISECONDS);

/**
 *	@brief Starts the clock so we can execute whatever we want and measure the used time.
 */
void initChrono();
}    // namespace ChronoUtilities

inline long long ChronoUtilities::getDuration(const TimeUnit timeUnit)
{
    const std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

    const long long measuredTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - _initTime).count();

    return measuredTime / timeUnit;
}

inline void ChronoUtilities::initChrono()
{
    _initTime = std::chrono::high_resolution_clock::now();
}
