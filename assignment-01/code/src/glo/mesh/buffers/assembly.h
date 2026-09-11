/*
 * glo/mesh/buffers/assembly.h
 *
 * Binds vertices, indices, normals, colours, and texture coordinates, to a
 * vertex array object. Shader locations are fixed:
 *     vertices: 0
 *     normals: 1
 *     colours: 2
 *     texcoords: 3
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <format>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../../error.h"
#include "../../log.h"
#include "../../scene/factories/geometric.h"
#include "../textures/image.h"
#include "buffer.h"
#include "colour.h"
#include "index.h"

class BufferAssembly
{
    friend std::ostream & operator<<(std::ostream & os,
        const BufferAssembly & obj);

  private:
    static constexpr GLuint VERTICES_POS = 0; // vertices' layout location
    static constexpr GLuint NORMALS_POS = 1; // normals' layout location
    static constexpr GLuint COLOURS_POS = 2; // colours' layout location
    static constexpr GLuint TEXCOORDS_POS = 3; // texcoords' layout location

  public:
    // the vertex buffer, and its draw drawMode, must be provided at
    // construction
    BufferAssembly(const GLenum drawMode, const Buffer & buffer);
    ~BufferAssembly();

    // prevent copying
    BufferAssembly(const BufferAssembly &) = delete;
    BufferAssembly & operator=(const BufferAssembly &) = delete;

    // allow move construction
    BufferAssembly(BufferAssembly &&);
    BufferAssembly & operator=(BufferAssembly &&);


    // === MUTATORS FOR INDICES, NORMALS, TEXTURE COORDINATES, AND COLOURS ===

    // clang-format off
    void setIndices(const IndexBuffer & buffer);
    void setNormals(const Buffer & buffer) { bind(NORMALS_POS, buffer); }
    void setTexCoords(const Buffer & buffer)
        { bind(TEXCOORDS_POS, buffer); }
    void setColours(GeometricFactory::ColourBufferPtr buffer)
        { bind(COLOURS_POS, *buffer); }
    // clang-format on


    // === BIND AND UNBIND THE VERTEX ARRAY OBJECT ===

    // clang-format off
    void bind() const { glBindVertexArray(id); GL_ERROR_CHECK; }
    void unbind() const { glBindVertexArray(0); GL_ERROR_CHECK; }
    // clang-format on

    // === RENDER OPERATIONS ===
    void draw(void) const;

    operator GLuint() const { return id; }

  private:
    // bind a buffer to the vertex array object
    void bind(const GLuint location, const Buffer & buffer);

    GLuint id = 0;
    GLsizei verticesCount = 0;
    GLsizei indicesCount = 0;
    GLenum drawMode = 0;
};
