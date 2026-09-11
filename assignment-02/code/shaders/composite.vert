/*
 * shaders/composite.vert
 *
 * Simply renders an image to a framebuffer. The QuadMesh class should be used
 * alongside this shader, as it already mapped to [-1.0,1.0] coordinates.
 *
 * © Tim Clarke November 2025
 */

#version 420 core

layout (location = 0) in vec2 a_Position;
layout (location = 3) in vec2 a_TextureCoord;

out vec2 v_TextureCoord;

void main()
{
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0); 
    v_TextureCoord = a_TextureCoord;
}
