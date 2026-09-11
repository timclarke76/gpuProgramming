/*
 * shaders/particles.vert
 *
 * Display a flat particle mesh to the screen, using bilboarding to ensure it is
 * aligned with the screen.
 */
#version 420 core

layout(location = 0) in vec3 a_LocalPos;
layout(location = 1) in vec3 a_WorldPos;
layout(location = 2) in vec4 a_Colour;
layout(location = 3) in float a_Diameter;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 v_Colour;
out vec2 v_FragPosition;

void main()
{
    // Convert the particle's centre to view space.
    vec3 viewPos = (u_View * vec4(a_WorldPos, 1.0)).xyz;

    // Offset in view space, ensuring the particle is aligned with the screen.
    vec3 offset = vec3(a_LocalPos.x * a_Diameter, a_LocalPos.y * a_Diameter,
        0.0);
    viewPos += offset;

    // Project to clip space.
    gl_Position = u_Projection * vec4(viewPos, 1.0);

    v_Colour = a_Colour;
    v_FragPosition = a_LocalPos.xy;
}
