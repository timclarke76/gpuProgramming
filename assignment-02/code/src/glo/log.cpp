/*
 * glo/log.cpp
 *
 * © Tim Clarke November 2025
 */

#include <chrono>
#include <format>
#include <iostream>

#include "log.h"

void
Log::print(const char * const colour, const char * severity,
    const char * const module, const std::string_view & s)
{
    std::cout << colour << getTimestamp() << " [" << severity << "] ["
        << module << "] " << s << RESET << std::endl;
    std::cout.flush();
}

/*
 * Calculate a timestamp, to the millisecond.
 */
std::string
Log::getTimestamp()
{
    const auto now = std::chrono::system_clock::now();
    const auto time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    char buffer[32] = {0};

    localtime_r(&time_t, &tm);
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);

    // clang-format off
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    // clang-format on

    return std::format("{}.{:03d}", buffer, ms.count());
}
