/*
 * shaders/lights.frag
 *
 * The fragment shader for the orbs, and the main light, to be used alongside
 * lights.vert.
 *
 * Simply return the in colour as an out.
 *
 * © Tim Clarke November 2025
 */

#version 420 core

in vec4 v_FragColour;

out vec4 o_Colour;

void main()
{
    o_Colour = v_FragColour;
}
