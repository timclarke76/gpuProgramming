/*
 * scene/torch.h
 *
 * The adventure torch provides power options (on/off), and four colour filters
 * alongside the bare bulb.  In addition, the beam width is adjustable --- the
 * attenuation is automatically adjusted as the beam width is narrowed or
 * windened.
 *
 * CITE: I had a discussion with DeekSeek (DeepSeek 2025) to find suitable
 * properties for an “adventure torch”. This included colours for the plastic
 * filters, and a colour for a bare incandescent bulb. I considered the beam
 * width and attenuation suggestions too large, so asked for pen light
 * properties and used those values instead.
 *
 * DeepSeek (2025). DeepSeek-V3.2-Exp. url: https://chat.deepseek.com
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <algorithm>

#include "../glo/colour.h"
#include "graphs/light_properties.h"
#include "support/circular_deque.h"

class Torch
{
        static constexpr GLfloat AMBIENT_INTENSITY = 0.1f;
        static constexpr GLfloat SPECULAR_INTENSITY = 0.5f;

    public:
        Torch();

        /*
         * The torch can be powered on and off.
         */
        Torch & togglePower() {powered = !powered; return *this;}
        bool isPowered() { return powered; }

        /*
         * The torch has four colour filters, plus the bare bulb, which rotate
         * in either direction.
         */
        Torch & rotateLensColour(const unsigned short direction)
            { colours.rotate(direction); return *this; }
        Colour & getLensColour() {return *colours; }

        /*
         * The beam width is adjustable.
         */
        Torch & adjustBeam(const GLfloat width);

        const LightProperties & getProperties() const {return properties;}

    private:
    bool powered = true;
    CircularDeque<Colour> colours;
    GLfloat beamWidth = 0.0f;

    LightProperties properties{{0.0f, 0.0f, 0.0f}, AMBIENT_INTENSITY,
        SPECULAR_INTENSITY, {0.0f, 0.0f}, {0.0f, 0.0f}};
};
