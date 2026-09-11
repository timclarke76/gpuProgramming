/*
 * glo/mesh/buffers/buffer.h
 *
 * An abstract buffer class, allowing BufferAssembly to interface with various
 * buffer implmentations, without knowing about the template design of the
 * BufferImpl class.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <format>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../../error.h"
#include "../../log.h"

class Buffer
{
  public:
    Buffer() = default;
    Buffer(const Buffer &) = delete;

    virtual ~Buffer()
    {
        if (id != 0)
        {
            glDeleteBuffers(1, &id);
            GL_ERROR_CHECK;
            Log::d("Buffer", std::format("VBO {} deleted", id));
        }
    }

    // bind to the buffer
    void bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        GL_ERROR_CHECK;
    }

    // unbind from the buffer
    void unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        GL_ERROR_CHECK;
    }

    // returns the number of components per generic vertex attribute
    virtual GLint getComponentCount() const = 0;

    // returns the data type of each component
    virtual GLenum getComponentType() const = 0;

    // returns whether or not the values should be normalised
    virtual GLboolean isNormalised() const = 0;

    // returns the numer of generic vertex attributes
    GLuint getCount() const { return count; }

    operator GLuint() const { return id; }

  protected:
    // the buffers id from OpenGL
    GLuint id = 0;

    // the number of generic vertex attributes
    GLuint count = 0;
};
