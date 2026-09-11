/*
 * glo/mesh/support/vertex_traits.h
 *
 * A support class for the BufferImpl teplate, providing component information
 * for vec2, vec3, and vec4 data classes.
 *
 * If another data class is required for BufferImpl, then it should be defined
 * here.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

template <typename T> struct VertexTraits;

/*
 * count: the number of components for the attribute of the type
 * type: the data type held within the type
 * isNormalised: whether or not the values should be normalised
 */
template <> struct VertexTraits<glm::vec2>
{
    static constexpr GLint count = 2;
    static constexpr GLenum type = GL_FLOAT;
    static constexpr GLboolean isNormalised = GL_FALSE;
};

template <> struct VertexTraits<glm::vec3>
{
    static constexpr GLint count = 3;
    static constexpr GLenum type = GL_FLOAT;
    static constexpr GLboolean isNormalised = GL_FALSE;
};

template <> struct VertexTraits<glm::vec4>
{
    static constexpr GLint count = 4;
    static constexpr GLenum type = GL_FLOAT;
    static constexpr GLboolean isNormalised = GL_FALSE;
};
