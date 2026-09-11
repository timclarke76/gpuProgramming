/*
 * scene/torch.cpp
 *
 * © Tim Clarke November 2025
 */

#include "torch.h"

/*
 * Construct our adventure torch, adding our bulb and four platic filters.
 */
Torch::Torch()
{
    adjustBeam(0.5f);

    colours.pushBack(Colour("#FFF4E3")); // incandescent bulb
    colours.pushBack(Colour("#FF2A2A")); // 🔴 red filter
    colours.pushBack(Colour("#32CD32")); // 🟢 green filter
    colours.pushBack(Colour("#1E90FF")); // 🔵 blue filter
    colours.pushBack(Colour("#FFD700")); // 🟡 yellow filter
}

Torch & 
Torch::adjustBeam(const GLfloat width)
{
    // clamp the width to a value between 0.0f and 1.0f
    beamWidth = std::clamp((beamWidth + width), 0.0f, 1.0f);

    // 8° to 30°
    // use glm::mix for a linear interpolation of our beam width
    const GLfloat innerAngle = glm::mix(3.0f, 15.0f, beamWidth);
    properties.cutOff.inner = glm::cos(glm::radians(innerAngle));

    // 18° to 60°
    // the outer cutOff is the faded outer circle propduced by a torch
    const GLfloat outerAngle = glm::mix(8.0f, 26.0f, beamWidth);
    properties.cutOff.outer = glm::cos(glm::radians(outerAngle));

    // use glm::mix again so wider beams have a shorter reach than narrower
    // beams
    properties.attenuation.linear = glm::mix(0.14f, 0.35f, beamWidth);
    properties.attenuation.quadratic = glm::mix(0.07f, 0.44f, beamWidth);

    return *this;
}
