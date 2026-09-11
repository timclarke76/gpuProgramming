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
 * a_Tangent (layout 4) and a_Bitangent (layout 5) are used to calculate v_TBN
 * for the fragment shader. They are ignored if u_HasNormals is false.
 *
 * © Tim Clarke November 2025
 */

#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Colour;
layout(location = 3) in vec2 a_TextureCoord;
layout(location = 4) in vec3 a_Tangent;
layout(location = 5) in vec3 a_Bitangent;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform bool u_HasNormals;

out vec3 v_FragPosition;
out vec3 v_Normal;
out vec4 v_Colour;
out vec2 v_TextureCoord;
out mat3 v_TBN;

void
main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

    v_FragPosition = vec3(u_Model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    v_Colour = a_Colour;
    v_TextureCoord = a_TextureCoord;

    if (u_HasNormals)
    {
        // Transform the TBN to world-space
        const vec3 T = normalize(mat3(u_Model) * a_Tangent);
        const vec3 B = normalize(mat3(u_Model) * a_Bitangent);
        const vec3 N = normalize(mat3(u_Model) * a_Normal);
        v_TBN = mat3(T, B, N);
    }
}
