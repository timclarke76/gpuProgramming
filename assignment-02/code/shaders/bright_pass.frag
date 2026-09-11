/*
 * shaders/bright_pass.frag
 *
 * Calculates the dot product of the colour and a luminance value to determine
 * the colour's brightness as perceived by the human eye, as the eye is more
 * sensitive to some colours than others. The brightness is then compared to
 * brightnessThreshold (which must be set by the client) to determine if it is
 * bright enough. If it is, we render the colour. Otherwise, we render black.
 *
 * CITE: I found the luminance vec3 in the Learn OpenGL guide, but did some
 * research into its meaning and original source. It originates from the HDTV
 * Recommendation ITU-R BT.709-6 standard for HDTV: https://www.itu.int/
 * dms_pubrec/itu-r/rec/bt/R-REC-BT.709-6-201506-I!!PDF-E.pdf
 *
 * © Tim Clarke November 2025
 */

#version 420 core

uniform sampler2D sceneTexture;
uniform float brightnessThreshold;

in vec2 v_TextureCoord;

out vec4 o_Colour;

void main()
{
    vec3 color = texture(sceneTexture, v_TextureCoord).rgb;

    // CITE: learnOpenGL, original source HDR Recommendation ITU-R BT.709-6
    float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));
    if (luminance < brightnessThreshold) color = vec3(0.0, 0.0, 0.0);

    o_Colour = vec4(color, 1.0);
}
