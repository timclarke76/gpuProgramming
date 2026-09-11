/*
 * glo/mesh/buffers/buffer_impl.h
 *
 * An implementation of the abstract buffer class. May be used with the glm
 * classes vec2, vec3, or vec4. Types may be extended by providing further
 * implementations of VertexTraits.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <format>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../../error.h"
#include "../../log.h"
#include "../support/vertex_traits.h"
#include "buffer.h"

/*
 * A VertexTraits<T> struct must be defined. Thus, T can be vec2, vec3, or vec4.
 */
template <typename T> class BufferImpl : public Buffer
{
    friend std::ostream & operator<< <T>(std::ostream & os,
        const BufferImpl<T> & obj);

  protected:
    /*
     * Create an empty buffer.  Data should be set before construction of the
     * derived class is complete.
     */
    BufferImpl()
    {
        glGenBuffers(1, &id);
        if (id == 0) throw std::runtime_error("Failed to create vertex buffer");
        Log::d("BufferImpl", std::format("VBO {} created", id));
    }

    /*
     * Sets the data to be stored in the buffer.
     * usage specifies the usage pattern of the data.
     */
    BufferImpl<T> & setData(const std::vector<T> & data,
        const GLenum usage = GL_STATIC_DRAW)
    {
        assert(!data.empty());

        count = data.size();
        bind();

        glBufferData(GL_ARRAY_BUFFER, count * sizeof(T), data.data(), usage);
        GL_ERROR_CHECK;

        return *this;
    }

  public:
    /*
     * Create a filled buffer.
     */
    BufferImpl(const std::vector<T> & data, const GLenum usage = GL_STATIC_DRAW)
        : BufferImpl()
    {
        setData(data, usage);
    }

    // don't allow copies
    BufferImpl(const BufferImpl<T> &) = delete;
    BufferImpl<T> & operator=(const BufferImpl<T> &) = delete;

    // move data to allow safe destruction of other (temporary object)
    BufferImpl(BufferImpl<T> && other)
    {
        if (id != 0)
        {
            glDeleteBuffers(1, &id);
            GL_ERROR_CHECK;
            Log::d("BufferImpl", std::format("VBO {} deleted", id));
        }

        id = std::exchange(other.id, 0);
        count = std::exchange(other.count, 0);
        traits = other.traits;
    }

    // move data to allow safe destruction of other (temporary object)
    BufferImpl<T> & operator=(BufferImpl<T> && other) noexcept
    {
        if (this != &other)
        {
            if (id != 0)
            {
                glDeleteBuffers(1, &id);
                GL_ERROR_CHECK;
                Log::d("BufferImpl", std::format("VBO {} deleted", id));
            }

            id = std::exchange(other.id, 0);
            count = std::exchange(other.count, 0);
            traits = other.traits;
        }

        return *this;
    }

    /*
     * Accessors to the VertexTraits<T> instance, allowing BufferAssembly to
     * determine the traits, without knowing about T.
     */
    // clang-format off

    // returns the number of components per generic vertex attribute
    virtual GLint getComponentCount() const override { return traits.count; }

    // returns the data type of each component
    virtual GLenum getComponentType() const override { return traits.type; }

    // returns whether or not the values should be normalised
    virtual GLboolean isNormalised() const override
        { return traits.isNormalised; }
    // clang-format on

  private:
    /*
     * A VertexTraits<T> support instance, providing information about T.
     */
    VertexTraits<T> traits;
};

template <typename T>
std::ostream &
operator<<(std::ostream & os, const BufferImpl<T> & obj)
{
    // clang-format off
    os << "{"
       <<     "\"_type\":\"BufferImpl\","
       <<     "\"id\":" << obj.id << ","
       <<     "\"count\":" << obj.count
       << "}";
    // clang-format on

    return os;
}
