/*
 * glo/mesh/buffers/index.cpp
 *
 * © Tim Clarke November 2025
 */

#include <cassert>
#include <stdexcept>

#include "../../log.h"

#include "index.h"

std::ostream &
operator<<(std::ostream & os, const IndexBuffer & obj)
{
    // clang-format off
    os << "{"
       <<     "\"_type\":\"IndexBuffer\","
       <<     "\"id\":" << obj.id << ","
       <<     "\"count\":" << obj.count
       << "}";
    // clang-format on

    return os;
}

/*
 * Creates an index buffer (an element array buffer) with the provided
 * indices data. The data must not be empty.
 *
 * usage specifies the expected usage pattern for this data
 */
IndexBuffer::IndexBuffer(const std::vector<GLuint> & data,
    const GLenum usage) : count(data.size())
{
    assert(!data.empty());

    // Allocate the VAO id.  If it fails, we cannot create a valid object,
    // so throw an exception.
    glGenBuffers(1, &id);
    if (id == 0) throw std::runtime_error("Failed to create index buffer");
    Log::d("IndexBuffer", std::format("EBO {} created", id));

    bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data.data(),
        usage);
    GL_ERROR_CHECK;
}

IndexBuffer::~IndexBuffer()
{
    if (id != 0)
    {
        glDeleteBuffers(1, &id);
        GL_ERROR_CHECK;
        Log::d("IndexBuffer", std::format("EBO {} deleted", id));
    }
}

/*
 * Provides safe moving from a temporary object.
 */
IndexBuffer::IndexBuffer(IndexBuffer && other)
{
    if (id != 0)
    {
        glDeleteBuffers(1, &id);
        GL_ERROR_CHECK;
        Log::d("IndexBuffer", std::format("EBO {} deleted", id));
    }

    id = std::exchange(other.id, 0);
    count = std::exchange(other.count, 0);
}

/*
 * Provides safe moving from a temporary object.
 */
IndexBuffer &
IndexBuffer::operator=(IndexBuffer && other)
{
    if (this != &other)
    {
        if (id != 0)
        {
            glDeleteBuffers(1, &id);
            GL_ERROR_CHECK;
            Log::d("IndexBuffer", std::format("EBO {} deleted", id));
        }

        id = std::exchange(other.id, 0);
        count = std::exchange(other.count, 0);
    }

    return *this;
}
