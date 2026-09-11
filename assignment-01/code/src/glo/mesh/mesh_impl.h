/*
 * glo/mesh/mesh_impl.h
 *
 * An implmentation of the abstract mesh class. May be used with any glm class
 * supported by BufferImpl --- currently vec2, vec3, or vec4.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <cassert>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "buffers/assembly.h"
#include "buffers/buffer_impl.h"
#include "buffers/colour.h"
#include "buffers/index.h"
#include "mesh.h"

template <typename T> class MeshImpl : public Mesh
{
  public:
    MeshImpl() = default;

    // no copying
    MeshImpl(const MeshImpl &) = delete;
    MeshImpl & operator=(const MeshImpl &) = delete;

    // safe moves
    MeshImpl(MeshImpl &&) = default;
    MeshImpl & operator=(MeshImpl &&) = default;

    /*
     * Sets vertices for a *new* vertex assembly object.
     *
     * mode: the required draw mode for these vertices.
     * data: the data containing the vertices
     */
    void setVertices(const GLenum mode, std::vector<T> data)
    {
        assert(!data.empty());

        count = data.size();

        vertices.emplace_back(data);
        assemblies.push_back(
            std::make_unique<BufferAssembly>(mode, vertices.back()));
    }

    /*
     * Sets indices for the *current* VAO. Only necessary for indexed vertices.
     * The vertices must already be set, and the data must not be empty and its
     * size divisible by 3.
     */
    void setIndices(std::vector<GLuint> data)
    {
        assert(!assemblies.empty());
        assert(!data.empty());
        assert(data.size() % 3 == 0);

        indices.emplace_back(data);
        assemblies.back()->setIndices(indices.back());
    }

    /*
     * Sets normals for the *current* VAO. Only necessary when normals are
     * required. The vertices must already be set, and the data must not be
     * empty.
     */
    void setNormals(std::vector<glm::vec3> data)
    {
        assert(!assemblies.empty());
        assert(!data.empty());

        normals.emplace_back(data);
        assemblies.back()->setNormals(normals.back());
    }

    /*
     * Sets texture coordinates for the *current* VAO. Only necessary when the
     * mesh has a texture. The vertices must already be set, and the data not be
     * empty.
     */
    void setTextureCoordinates(std::vector<glm::vec2> data)
    {
        assert(!assemblies.empty());
        assert(!data.empty());

        texCoords.emplace_back(data);
        assemblies.back()->setTexCoords(texCoords.back());
    }

    /*
     * Sets the ColourBuffer for our VAO.  A reference to the buffer is not
     * retained. This is typically called by the mesh client, and not set by any
     * derived class. The vertices must already be set.
     */
    virtual void setColours(GeometricFactory::ColourBufferPtr buffer) override
    {
        assert(!assemblies.empty());
        assemblies.back()->setColours(buffer);
    }

    /*
     * Draws each of our assemblies to screen. If no vertices have yet been set,
     * no draw operations will take place.
     */
    void draw() const override
    {
        for (const auto & a : assemblies) a->draw();
    }

  protected:
    /*
     * Convenience method for derived classes, to allow easier specification of
     * vertices.
     *
     * Creates a deindexed vector, by combining the given vertices and indices.
     * Each index must reference one of the given vertices.
     *
     * Both the vertices and the indices must contain data. The size of the
     * indices must be divisible by 3.
     */
    static std::vector<T> deindex(const std::vector<T> & vertices,
        const std::vector<GLuint> & indices)
    {
        assert(!vertices.empty());
        assert(!indices.empty());
        assert(indices.size() % 3 == 0);

        std::vector<T> deindexed;
        deindexed.reserve(indices.size());

        for (auto it = indices.begin(); it != indices.end(); it += 3)
        {
            // === Gather the trio of indexes, and verify their validity ===
            const GLuint i0 = *it;
            assert(i0 < vertices.size());

            const GLuint i1 = *(it + 1);
            assert(i1 < vertices.size());

            const GLuint i2 = *(it + 2);
            assert(i2 < vertices.size());

            // === Copy the referenced vertices to the deindexed vector ===
            deindexed.push_back(vertices[i0]);
            deindexed.push_back(vertices[i1]);
            deindexed.push_back(vertices[i2]);
        }

        return deindexed;
    }

    /*
     * Convenience method for derived classes, to calculate a vector of normals
     * for the provided _deindexed_ vertices.
     *
     * The deindexed data must not be empty, and its count divisible by 3.
     */
    static std::vector<glm::vec3> calculateNormals(
        const std::vector<T> & deindexed)
    {
        assert(!deindexed.empty());
        assert((deindexed.size() % 3) == 0);

        std::vector<glm::vec3> normals;
        normals.reserve(deindexed.size());

        for (auto it = deindexed.begin(); it != deindexed.end(); it += 3)
        {
            // === Gather the three vertices of the next plane. ===
            const auto & a = *it;
            const auto & b = *(it + 1);
            const auto & c = *(it + 2);

            // === Calculate two of the plane's edges by subtracting one ===
            // === vertex from another.                                  ===
            const auto edge1 = (glm::vec3(b) - glm::vec3(a));
            const auto edge2 = (glm::vec3(c) - glm::vec3(a));

            // === Cross product the two edges to calculate the normal ===
            // === direction, and normalize the result.                ===
            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

            // === Store the calculated normal for each vertex of the ===
            // === current plane.                                     ===
            for (auto i = 0; i < 3; i++) normals.push_back(normal);
        }

        return normals;
    }

    /*
     * Convenience method for derived classes to calculate a texture offset
     * between 0.0 and 1.0.
     *
     * For example, if the client needs a UV coordinate one-third of the way
     * into a 768x1024 image:
     *     getUVCoord(768, 1, 3) will provide the horizontal coord
     *     getUVCoord(1024, 1, 3) will provide the vertical coord
     *
     * total: the total width or height of the image
     * numerator: the numerator of the client's fraction
     * denominator: the denominator of the client's fraction
     *
     * returns: the UV coord, between 0.0 and 1.0
     */
    static GLfloat getUVCoord(const GLuint total, const GLuint numerator,
        const GLuint denominator)
    {
        return GLfloat((total * numerator) / denominator) / total;
    }

  private:
    // vectors allow the use of multiple draw calls
    std::vector<std::unique_ptr<BufferAssembly>> assemblies;
    std::vector<BufferImpl<T>> vertices;
    std::vector<IndexBuffer> indices;
    std::vector<BufferImpl<glm::vec3>> normals;
    std::vector<BufferImpl<glm::vec2>> texCoords;
};
