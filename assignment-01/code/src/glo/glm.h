/*
 * glo/glm.h
 *
 * ostream operators for vec2, vec3, vec4, and mat4 instances. Strings are
 * generated in a JSON compatible format.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <iostream>

#include <glm/glm.hpp>

inline std::ostream &
operator<<(std::ostream & os, const glm::vec2 & obj)
{
    // clang-format off
    os << "["
       <<     obj.s << ","
       <<     obj.t
       << "]";
    // clang-format on

    return os;
}

inline std::ostream &
operator<<(std::ostream & os, const glm::vec3 & obj)
{
    // clang-format off
    os << "["
       <<     obj.x << ","
       <<     obj.y << ","
       <<     obj.z
       << "]";
    // clang-format on

    return os;
}

inline std::ostream &
operator<<(std::ostream & os, const glm::vec4 & obj)
{
    // clang-format off
    os << "["
       <<     obj.r << ","
       <<     obj.g << ","
       <<     obj.b << ","
       <<     obj.a
       << "]";
    // clang-format on

    return os;
}

inline std::ostream &
operator<<(std::ostream & os, const glm::mat4 & obj)
{
    os << "[";

    for (auto row = 0; row < 4; row++)
    {
        os << "[";

        for (auto col = 0; col < 4; col++)
        {
            os << obj[col][row];
            if (col < 3) os << ",";
        }

        os << "]";
        if (row < 3) os << ",";
    }

    os << "]";

    return os;
}
