/*
 * shaders/lights.vert
 *
 * The vertice shader for the orbs, and the main light, to be used alongside
 * lights.frag.
 *
 * These shaders don't deal with the emitted light --- they just draw the object
 * itself. No textures are used.
 *
 * Positions are expected in vec3 format, as w is always 1.0 for our program so
 * sending it to the GPU would be redundant. Instead, we set w to 1.0 here.
 *
 * © Tim Clarke November 2025
 */

#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Colour;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec4 v_FragColour;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
    v_FragColour = a_Colour;
}
