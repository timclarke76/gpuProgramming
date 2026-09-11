/*
 * shaders/skybox.vert
 *
 * The vertice shader for the skybox, to be used alongside skyboxfrag.
 *
 * These shaders are responsible for rendering our skybox, which must be
 * supplied as a texture cube.
 *
 * The view location is converted from a mat4 into a mat3, then back to a mat4.
 * This has the effect of removing any translations.
 *
 * We also use position.xyww, having the effect of the depth position always at
 * 1.0 (as normalising the device coordinates invloves dividing xyz by w, thus
 * if z is equal to w, the result will always be 1.0).  1.0 is the maximum depth
 * value.
 *
 * © Tim Clarke November 2025
 */

#version 420 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_Projection;
uniform mat4 u_View;

out vec3 v_TextureCoord;

void main()
{
    vec4 position = u_Projection * mat4(mat3(u_View)) * vec4(a_Position, 1.0);
    gl_Position = position.xyww;

    v_TextureCoord = a_Position;
}
