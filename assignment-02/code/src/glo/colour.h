/*
 * glo/colour.h
 *
 * A basic colour class to convert colour hex strings to rgba floats.
 *
 * The # at the start of the string is optional. The remainder of the string
 * must be 3, 4, 6, or 8 characters in length:
 *   3: RGB
 *   4: RGBA
 *   6: RRGGBB
 *   8: RRGGBBAA
 *
 * If alpha is not supplied, then it is assumed to be FF.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Colour
{
    friend std::ostream & operator<<(std::ostream & os, const Colour & obj);

  public:
    Colour(const std::string & hex);
    Colour(const Colour &) = default;
    Colour & operator=(const Colour &) = default;

    // clang-format off
    bool operator==(const Colour & rhs) const
        { return getHex() == rhs.getHex(); }
    // clang-format on

    bool operator!=(const Colour & rhs) const { return !(*this == rhs); }

    const std::string & getHex() const { return hex; }
    const glm::vec4 & getRgba() const { return rgba; }
    const glm::vec3 getRgb() const { return glm::vec3(rgba); }

    static std::string toHex(const float r, const float g, const float b,
        const float a = 1.0f);

  private:
    std::string hex;
    glm::vec4 rgba;
};
