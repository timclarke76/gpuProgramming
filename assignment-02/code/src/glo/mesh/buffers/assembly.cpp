/*
 * glo/mesh/buffers/assembly.cpp
 *
 * © Tim Clarke November 2025
 */

#include <stdexcept>

#include "assembly.h"

/*
 * Debug output, in JSON format.
 */
std::ostream &
operator<<(std::ostream & os, const BufferAssembly & obj)
{
    // clang-format off
    os << "{"
       <<     "\"_type\":\"BufferAssembly\","
       <<     "\"id\":" << obj.id << ","
       <<     "\"verticesCount\":" << obj.verticesCount << ","
       <<     "\"indicesCount\":" << obj.indicesCount << ","
       <<     "\"drawMode\":" << obj.drawMode
       << "}";
    // clang-format on

    return os;
}

/*
 * drawMode: GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES,
 * GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP,
 * GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY,
 * GL_TRIANGLES_ADJACENCY or GL_PATCHES
 *
 * buffer: Our vertices.  If this is indexed, a call to setIndices will
 * be required by the client.
 */
BufferAssembly::BufferAssembly(const GLenum drawMode, const Buffer & buffer)
    : drawMode(drawMode)
{
    // Allocate the VAO id.  If it fails, we cannot create a valid object,
    // so throw an exception.
    glCreateVertexArrays(1, &id);
    if (id == 0) throw std::runtime_error("Failed to create VAO id");
    Log::d("BufferAssembly", std::format("VAO {} created", id));
    GL_ERROR_CHECK;

    // Retain the buffer count, for our draw operation.
    verticesCount = buffer.getCount();
    assert(verticesCount != 0);

    // Bind the buffer to the VAO.
    bind(VERTICES_POS, buffer);
}

BufferAssembly::~BufferAssembly()
{
    if (id != 0)
    {
        glDeleteVertexArrays(1, &id);
        GL_ERROR_CHECK;
        Log::d("BufferAssembly", std::format("VAO {} deleted", id));
    }
}

/*
 * Move constructor.  Take the values from other, and reset other's values
 * to ensure safe destruction.
 */
BufferAssembly::BufferAssembly(BufferAssembly && other)
{
    if (id != 0)
    {
        glDeleteVertexArrays(1, &id);
        GL_ERROR_CHECK;
        Log::d("BufferAssembly", std::format("VAO {} deleted", id));
    }

    id = std::exchange(other.id, 0);
    verticesCount = std::exchange(other.verticesCount, 0);
    indicesCount = std::exchange(other.indicesCount, 0);
    drawMode = std::exchange(other.drawMode, 0);
}

/*
 * Move operator.  Take the values from other, and reset other's values
 * to ensure safe destruction.
 */
BufferAssembly &
BufferAssembly::operator=(BufferAssembly && other)
{
    if (this != &other)
    {
        if (id != 0)
        {
            glDeleteVertexArrays(1, &id);
            GL_ERROR_CHECK;
            Log::d("BufferAssembly", std::format("VAO {} deleted", id));
        }

        id = std::exchange(other.id, 0);
        verticesCount = std::exchange(other.verticesCount, 0);
        indicesCount = std::exchange(other.indicesCount, 0);
        drawMode = std::exchange(other.drawMode, 0);
    }

    return *this;
}

/*
 * Set the index buffer for the vertices. The vertexes must be indexed.
 */
void
BufferAssembly::setIndices(const IndexBuffer & buffer)
{
    bind();
    buffer.bind();

    // retain the count for our draw operation
    indicesCount = buffer.getCount();
}

/*
 * Draw the VAO to the screen.
 */
void
BufferAssembly::draw() const
{
    bind();

    if (indicesCount != 0)
    {
        // If we have an indicesCount, then the vertices must be indexed.
        glDrawElements(drawMode, indicesCount, GL_UNSIGNED_INT, nullptr);
        GL_ERROR_CHECK;
    }
    else
    {
        // Otherwise, the vertices are not indexed. We use verticesCount
        // instead.
        glDrawArrays(drawMode, 0, verticesCount);
        GL_ERROR_CHECK;
    }
}

/*
 * Bind a vertex buffer attached to the VAO to the given location, and enable
 * it.
 */
void
BufferAssembly::bind(const GLuint location, const Buffer & buffer)
{
    bind();
    buffer.bind();

    glVertexAttribPointer(location, buffer.getComponentCount(),
        buffer.getComponentType(), buffer.isNormalised(), 0, nullptr);
    glEnableVertexAttribArray(location);
    GL_ERROR_CHECK;
}
