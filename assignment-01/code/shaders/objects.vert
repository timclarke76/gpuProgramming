/*
 * shaders/objects.vert
 *
 * The vertice shader for the cot mobile, to be used alongside objects.frag.
 *
 * These shaders draw the various pieces of the cot mobile, and colour the
 * framents according to colour, texture, and light properties.
 *
 * Positions are expected in vec3 format, as w is always 1.0 for our program so
 * sending it to the GPU would be redundant. Instead, we set w to 1.0 here.
 *
 * Because the fragment shader considers the impact of the lights on the colour,
 * we need to pass it the fragment's position and normal. Objects may have a
 * texture, so we also send both the colour and the texture coordinate.
 *
 * © Tim Clarke November 2025
 */

#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Colour;
layout(location = 3) in vec2 a_TextureCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec3 v_FragPosition;
out vec3 v_Normal;
out vec4 v_Colour;
out vec2 v_TextureCoord;

void
main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

    v_FragPosition = vec3(u_Model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    v_Colour = a_Colour;
    v_TextureCoord = a_TextureCoord;
}
