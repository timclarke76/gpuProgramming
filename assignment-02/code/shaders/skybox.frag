/*
 * shaders/skybox.frag
 *
 * The fragment shader for the skybox. Simply return the texture's colour at the
 * given coordinate.
 *
 * © Tim Clarke November 2025
 */

#version 420 core

uniform samplerCube u_Texture;

in vec3 v_TextureCoord;

out vec4 o_Colour;

void main()
{
    o_Colour = texture(u_Texture, v_TextureCoord);
}
