/*
 * glo/mesh/buffers/index.h
 *
 * Holds indices information for an assembly vertex array object.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <utility>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../../error.h"

class IndexBuffer
{
    friend std::ostream & operator<<(std::ostream & os,
        const IndexBuffer & obj);

  public:
    /*
     * Must always hold data.
     * usage: specifies the usage pattern of the data.
     */
    IndexBuffer(const std::vector<GLuint> & data,
        const GLenum usage = GL_STATIC_DRAW);
    ~IndexBuffer();

    // copying not allowed
    IndexBuffer(const IndexBuffer &) = delete;
    IndexBuffer & operator=(const IndexBuffer &) = delete;

    // safely move
    IndexBuffer(IndexBuffer && other);
    IndexBuffer & operator=(IndexBuffer && other);

    void bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        GL_ERROR_CHECK;
    }

    void unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        GL_ERROR_CHECK;
    }

    // returns the number of indices in the buffer
    GLsizei getCount() const { return count; }

  private:
    // the OpenGL allocated id
    GLuint id = 0;

    // the number of indices in the buffer
    GLsizei count = 0;
};
