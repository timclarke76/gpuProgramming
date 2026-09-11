/*
 * scene/graphs/light_properties.h
 *
 * Defines the characteristics of a light, to allow desired rendering by the
 * shaders.
 *
 * CITE: The Ogre3d wiki (Point Light Attenuation n.d.) was used for liner and
 * quadratic values for various light ranges.
 *
 * CITE: I asked DeepSeek (DeepSeek 2025) to extrapolate linear and quadratic
 * values from the data for ranges that I felt more desirable.
 *
 * DeepSeek (2025). DeepSeek-V3.2-Exp. url: https://chat.deepseek.com
 *
 * Point Light Attenuation (n.d.). OGRE Wiki.
 *   url: http://wiki.ogre3d.org:0/-Point+Light+Attenuation
 *
 * © Tim Clarke November 2025
 */

#pragma once

/*
 * Attenuation defines how far the lights shines, by providing fall off rates.
 *
 * Constant remains at 1.0 throughout
 *
 * Range  Linear  Quadratic
 * 3250   0.0014  0.000007
 *  600   0.007   0.0002
 *  325   0.014   0.0007
 *  200   0.022   0.0019
 *  160   0.027   0.0028
 *  100   0.045   0.0075
 *   65   0.07    0.017
 *   50   0.09    0.032
 *   32   0.14    0.07
 *   20   0.22    0.20
 *   13   0.35    0.44
 *    7   0.7     1.86
 *
 * Extrapolated:
 *
 * Range  Linear  Quadratic
 * 5.00   0.18     0.32
 * 2.50   0.60     0.8
 * 2.00   1.00     1.0
 * 1.50   1.50     2.0
 * 0.25   8.00    16.0
 *   󰛤    0.00     0.0
 */
struct Attenuation
{
    GLfloat linear;
    GLfloat quadratic;
};

/*
 * The width of the light's beam. Use 0.0f for both values when there is no
 * cutoff.
 */
struct CutOff
{
    GLfloat inner;
    GLfloat outer;
};

struct LightProperties
{
    glm::vec3 direction; // the light's direction, or {0.0f} for no direction
    GLfloat ambientIntensity; // how strong the light's ambient intensity is
    GLfloat specularIntensity; // how strong the light's specular intensity is
    Attenuation attenuation;
    CutOff cutOff;
};
