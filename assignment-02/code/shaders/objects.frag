/*
 * shaders/objects.frag
 *
 * The fragment shader for the cot mobile, to be used alongside objects.vert.
 *
 * These shaders draw the various pieces of the cot mobile, and colour the
 * framents according to colour, texture, and light properties.
 *
 * The Phong method is used for lighting, with Blinn Phong Specular if
 * u_UseBlinnPhongSpecular is non-zero.
 *
 * Up to seven lights are supported. The total number of lights in the scene
 * should be communicated to the fragment shader through u_NumLights. Each light
 * should have its properties set in u_Lights[].
 *
 * © Tim Clarke November 2025
 */

#version 420 core

/*
 * How strong a light source is over given distances.
 */
struct Attenuation {
    float linear;
    float quadratic;
};

/*
 * The angle of a light beam's inner and outer cones.
 */
struct CutOff {
    float inner;
    float outer;
};

/*
 * All the properties of a given light source necessary to calculate it's colour
 * and strength at the fragment's position
 */
struct Light {
    vec3 position;
    vec3 direction;
    vec3 colour;
    float ambientIntensity;
    float specularIntensity;
    Attenuation attenuation;
    CutOff cutOff;
};

uniform Light u_Lights[7];
uniform int u_NumLights;

uniform vec3 u_Position;
uniform bool u_HasTexture;
uniform bool u_HasNormals;
uniform int u_UseBlinnPhongSpecular;

uniform sampler2D u_Texture;
uniform sampler2D u_Normals;

in vec3 v_FragPosition;
in vec4 v_Colour;
in vec3 v_Normal;
in vec2 v_TextureCoord;
in mat3 v_TBN;

out vec4 o_Colour;

/*
 * Calculates the first Phong component --- ambient lighting.
 *
 * The ambient intensity of the light is multiplied by its colour.
 */
vec3 getAmbient(Light light)
{
    return light.ambientIntensity * light.colour;
}

/*
 * Calculates the second Phong component --- diffused lighting.
 *
 * Multiplies the light's colour by the dot product of the surface's normal, and
 * the direction of the light relative to the fragment, when n·d is ≥ 0.0
 */
vec3 getDiffusion(Light light, vec3 normal, vec3 direction)
{
    float lambertFactor = max(dot(normal, direction), 0.0);
    return (lambertFactor * light.colour);
}

/*
 * Calculates the third Phong component --- specular --- using the original 1975
 * equation.
 *
 * TODO: shinines should be an attribute of material, and passed to the
 * shaders as a uniform.
 */
vec3 getPhongSpecular(Light light, vec3 normal, vec3 direction)
{
    vec3 viewDirection = normalize(u_Position - v_FragPosition);
    vec3 reflectDirection = reflect(-direction, normal);
    float specularAngle = max(dot(viewDirection, reflectDirection), 0.0);
    float specularPower = pow(specularAngle, /* shininess */ 32);

    return light.specularIntensity * specularPower * light.colour;
}

/*
 * Calculates the third Phong component --- specular --- using Blinn's 1977
 * modification to the original equation.
 *
 * TODO: shinines should be an attribute of material, and passed to the
 * shaders as a uniform.
 */
vec3 getBlinnPhongSpecular(Light light, vec3 normal, vec3 direction)
{
    vec3 viewDirection = normalize(u_Position - v_FragPosition);
    vec3 halfwayDirection = normalize(direction + viewDirection);
    float specularAngle = max(dot(normal, halfwayDirection), 0.0);
    float specularPower = pow(specularAngle, /* shininess */ 32);

    return light.specularIntensity * specularPower * light.colour;
}

/*
 * Returns the third Phong component --- specular. If u_UseBlinnPhongSpecular is
 * 0, then the original equation is used. Otherwise, Blinn's modification is
 * used instead.
 */
vec3 getSpecular(Light light, vec3 normal, vec3 direction)
{
    if (u_UseBlinnPhongSpecular == 0)
    {
        return getPhongSpecular(light, normal, direction);
    }
    else
    {
        return getBlinnPhongSpecular(light, normal, direction);
    }
}

/*
 * Calculates the attenuation value over the given distance for the light's
 * attenuation linear and quadratic (which when used together gives us a range
 * with degrading intensity).
 */
float getAttenuation(Light light, float distance)
{
    return 1.0 / (1.0 + light.attenuation.linear * distance
        + light.attenuation.quadratic * (distance * distance));
}

void main()
{
    vec4 fragColour;

    /*
     * Grabs the colour from the texture if we have one, otherwise uses
     * in v_Colour.
     */
    if (u_HasTexture) fragColour = texture(u_Texture, v_TextureCoord);
    else fragColour = v_Colour;

    /*
     * If the colour is fully transparent, discard this fragment, ending
     * execution.
     */
    if (fragColour.a == 0.0) discard;

    vec3 normal = normalize(v_Normal);

    if (u_HasNormals)
    {
        vec3 textureNormal = texture(u_Normals, v_TextureCoord).rgb * 2.0 - 1.0;
        textureNormal = normalize(v_TBN * textureNormal);
        normal = normalize(normal + textureNormal * 0.5);
    }

    vec3 totalLighting = vec3(0.0);

    /*
     * Add up the total Phong illumination for all the lights.
     */
    for (int i = 0; i < u_NumLights; i++)
    {
        Light light = u_Lights[i];

        vec3 direction = normalize(light.position - v_FragPosition);
        float distance = length(light.position - v_FragPosition);
        float intensity = 1.0;

        /*
         * Calculates a smooth intensity cut off for spotlights, using an inner
         * and outer cone. Diffusion and specular should both be multiplied by
         * this intensity value. Lights which are not spotlights will be
         * multiplied by the default valuf of 1.0, having no effect on the
         * result.
         *
         * CITE: Vries, J. de (2020) Learn OpenGL: Learn modern OpenGL graphics
         * programming in a step-by-step fashion. Erscheinungsort nicht
         * ermittelbar: Kendall & Wells.
         */
        if (light.cutOff.inner > 0.0)
        {
            float theta = dot(direction, normalize(-light.direction));
            float epsilon = light.cutOff.inner - light.cutOff.outer;
            intensity = clamp((theta - light.cutOff.outer) / epsilon, 0.0, 1.0);
        }

        /*
         * Calculate the Phong illumination, and adjust by any spotlight
         * intensity.
         */
        vec3 lighting = getAmbient(light);
        lighting += intensity * getDiffusion(light, normal, direction);
        lighting += intensity * getSpecular(light, normal, direction);

        /*
         * Adjust this light's total value for distance falloff.
         */
        lighting *= getAttenuation(light, distance);

        /*
         * And add it to the total.
         */
        totalLighting += lighting;
    }

    /*
     * Clamp the total result to ensure it's within the valid range.
     */
    totalLighting = clamp(totalLighting, 0.0, 1.0);

    /*
     * And apply the total lighting to our original fragment colour (which may
     * or may not be from a texture).
     */
    o_Colour = vec4(totalLighting, 1.0) * fragColour;
}
