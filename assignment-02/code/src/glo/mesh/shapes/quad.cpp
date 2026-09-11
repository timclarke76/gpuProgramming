/*
 * glo/mesh/shapes/quad.cpp
 *
 * © Tim Clarke November 2025
 */

#include "quad.h"

#include <vector>

/*
 * Constructs a quad mesh, suitable for rendering to a framebuffer.
 */
QuadMesh::QuadMesh()
{
    // Calculate all of our vertices

    // clang-format off
    const auto vertices = std::vector<glm::vec2>({
        {-RADIUS, -RADIUS}, // bottom-left
        { RADIUS, -RADIUS}, // bottom-right
        { RADIUS,  RADIUS}, // top-right
        {-RADIUS,  RADIUS}, // top-left
    });

    const auto indices = std::vector<GLuint>({
        0, 1, 2,
        2, 3, 0
    });

    // clang-format on
    setVertices(GL_TRIANGLES, deindex(vertices, indices));

    auto texCoords = std::vector<glm::vec2>();
    texCoords.reserve(indices.size());

    // clang-format off
    texCoords.insert(texCoords.end(), {
        {0.0f, 0.0f}, // bottom-left
        {1.0f, 0.0f}, // bottom-right
        {1.0f, 1.0f}, // top-right
        {0.0f, 1.0f}, // top-left
    });
    // clang-format on

    setTextureCoordinates(deindex(texCoords, indices));
}
