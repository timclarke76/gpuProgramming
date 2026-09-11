/*
 * shaders/particles.frag
 *
 * v_FragPosition is in local space, with range [-0.5,0.5]. By comparing its
 * length and discarding any vectors longer than 0.5, we draw a circle.
 *
 * o_Colour remains unchanged from v_Colour, though we add some alpha at the
 * edge of the particles to avoid aliasing. Note that particles are not
 * affected by light sources.
 */
#version 420 core

in vec4 v_Colour;
in vec2 v_FragPosition;

out vec4 o_Colour;

void main()
{
    float dist = length(v_FragPosition);
    if (dist > 0.5) discard;

    o_Colour = v_Colour;

    // fade a little at the edge to avoid aliasing
    o_Colour.a *= smoothstep(0.5, 0.48, dist);
}
