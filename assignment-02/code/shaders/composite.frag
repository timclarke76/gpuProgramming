/*
 * shaders/composite.frag
 *
 * Takes a scene, and a created bloom effect, and combines them together.
 * This is our final render, so any alpha values are ignored.
 *
 * bloomStrength should be set by the client to dictate how strong the final
 * bloom effect will be.
 *
 * toneMappingTechnique should be set by the client to dictate which tone
 * mapping technique, if any, should be applied.
 *
 * © Tim Clarke November 2025
 */

#version 420 core

uniform sampler2D sceneTexture;
uniform sampler2D bloomTexture;
uniform float bloomStrength;
uniform int toneMappingTechnique;

in vec2 v_TextureCoord;

out vec4 o_Colour;

vec3 reinhard(vec3 colour)
{
    return colour / (colour + vec3(1.0));
}

// CITE: https://knarkowicz.wordpress.com/2016/01/06/
// aces-filmic-tone-mapping-curve/
vec3 aces(const vec3 colour)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;

    return clamp((colour * (a * colour + b)) / (colour * (c * colour + d) + e),
        0.0, 1.0);
}

// CITE: http://filmicworlds.com/blog/filmic-tonemapping-operators/
vec3 uncharted2ToneMap(const vec3 colour)
{
    const float A = 0.15;
    const float B = 0.50;
    const float C = 0.10;
    const float D = 0.20;
    const float E = 0.02;
    const float F = 0.30;

    return ((colour * (A * colour + C * B) + D * E) /
        (colour * (A * colour + B) + D * F)) - E / F;
}

vec3 uncharted2(vec3 colour)
{
    const float W = 11.2;
    const float exposureBias = 2.0;
    const vec3 curr = uncharted2ToneMap(exposureBias * colour);
    const vec3 whiteScale = 1.0 / uncharted2ToneMap(vec3(W));

    return (curr * whiteScale);
}

void main()
{
    const vec3 scene = texture(sceneTexture, v_TextureCoord).rgb;
    const vec3 bloom = texture(bloomTexture, v_TextureCoord).rgb;
    vec3 result = scene + (bloom * bloomStrength);

    switch (toneMappingTechnique)
    {
        case 1: result = reinhard(result); break;
        case 2: result = aces(result); break;
        case 3: result = uncharted2(result); break;
        default: break;
    }

    o_Colour = vec4(result, 1.0);
}
