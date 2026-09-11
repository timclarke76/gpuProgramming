/*
 * glo/colour.cpp
 *
 * © Tim Clarke November 2025
 */

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "glm.h"

#include "colour.h"

std::ostream &
operator<<(std::ostream & os, const Colour & obj)
{
    // clang-format off
    os << "{"
       <<     "\"_type\":\"Colour\","
       <<     "\"hex\":\"" << obj.hex << "\","
       <<     "\"rgba\":" << obj.rgba
       << "}";
    // clang-format on

    return os;
}

Colour::Colour(const std::string & hex)
{
    std::string h = hex;

    // discard the optional # character
    if (!h.empty() && h[0] == '#') h = h.substr(1);

    switch (h.length())
    {
        // supplied in RGB format
        case 3: h = {h[0], h[0], h[1], h[1], h[2], h[2], 'F', 'F'}; break;

        // supplied in RGBA format
        case 4: h = {h[0], h[0], h[1], h[1], h[2], h[2], h[3], h[3]}; break;

        // supplied in RRGGBB format
        case 6: h += "FF"; break;

        // supplied in RRGGBBAA format --- no modifications necessary
        case 8: break;

        // the supplied string is not a colour
        default:
        {
            throw std::invalid_argument("Invalid hex colour: \"" + hex + "\"");
        }
        break;
    }

    unsigned long value;

    try
    {
        value = std::stoul(h, nullptr, 16);
    }
    catch (const std::exception &)
    {
        // the supplied string is not base 16
        throw std::invalid_argument("Invalid hex colour: \"" + hex + "\"");
    }

    // uppercase the string, for comparison operations
    std::transform(h.begin(), h.end(), h.begin(), ::toupper);
    this->hex = h;

    // bit shifting and masking to split the unsigned long into four ints
    // clang-format off
    rgba = {
        (((value >> 24) & 0xFF) / 255.0f),
        (((value >> 16) & 0xFF) / 255.0f),
        (((value >>  8) & 0xFF) / 255.0f),
        ((value & 0xFF) / 255.0f)};
    // clang-format on
}

/*
 * Convert rgba values (0.0f to 1.0f) to a hex string.
 */
std::string
Colour::toHex(const float r, const float g, const float b, const float a)
{
    std::stringstream ss;

    // clang-format off
    ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(r * 255)
       << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(g * 255)
       << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(b * 255);
    // clang-format on

    return ss.str();
}
