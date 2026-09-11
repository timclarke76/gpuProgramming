/*
 * glo/mesh/shapes/cube.cpp
 *
 * © Tim Clarke November 2025
 */

#include <vector>

#include "cube.h"

/*
 * Constructs a cube mesh, size 1x1x1.
 *
 * All eight vertex positions are calculated manually, followed by a vector of
 * indexes. The deindex method is then used to provide our final vector of
 * vertics, to be used by OpenGL.
 *
 * Normals are automatically calculated from the deindexed vertices, using the
 * calculateNormals method.
 *
 * Texture coordinates are calculated based on the industry stanard cube wrap
 * cross. These are manually specified.
 */
CubeMesh::CubeMesh()
{
    // Calculate all of our vertices

    // clang-format off
    const auto vertices = std::vector<glm::vec3>({
        // front
        {-RADIUS, -RADIUS,  RADIUS},
        { RADIUS, -RADIUS,  RADIUS},
        { RADIUS,  RADIUS,  RADIUS},
        {-RADIUS,  RADIUS,  RADIUS},
        
        // back
        { RADIUS, -RADIUS, -RADIUS},
        {-RADIUS, -RADIUS, -RADIUS},
        {-RADIUS,  RADIUS, -RADIUS},
        { RADIUS,  RADIUS, -RADIUS},
    });

    // Specify how each vertex position should be used to construct a cube

    const auto indices = std::vector<GLuint>({
        0, 1, 2, 0, 2, 3, // front
        1, 4, 7, 1, 7, 2, // right
        4, 5, 6, 4, 6, 7, // back
        5, 0, 3, 5, 3, 6, // left
        3, 2, 7, 3, 7, 6, // top
        1, 0, 5, 1, 5, 4  // bottom
    });

    // And deindex the vertices into data to be used by OpenGL

    // clang-format on
    auto deindexed = deindex(vertices, indices);
    setVertices(GL_TRIANGLES, deindexed);
    setNormals(calculateNormals(deindexed));

    auto texCoords = std::vector<glm::vec2>();
    texCoords.reserve(indices.size());

    const auto t1 = getUVCoord(768, 1, 3);
    const auto t2 = getUVCoord(768, 2, 3);

    // clang-format off
    texCoords.insert(texCoords.end(), {
        {0.25f,    t2}, {0.50f,    t2}, {0.50f,    t1}, // front
        {0.25f,    t2}, {0.50f,    t1}, {0.25f,    t1},
        {0.50f,    t2}, {0.75f,    t2}, {0.75f,    t1}, // right
        {0.50f,    t2}, {0.75f,    t1}, {0.50f,    t1},
        {0.75f,    t2}, {1.00f,    t2}, {1.00f,    t1}, // back
        {0.75f,    t2}, {1.00f,    t1}, {0.75f,    t1},
        {0.00f,    t2}, {0.25f,    t2}, {0.25f,    t1}, // left
        {0.00f,    t2}, {0.25f,    t1}, {0.00f,    t1},
        {0.25f,    t1}, {0.50f,    t1}, {0.50f, 0.00f}, // top
        {0.25f,    t1}, {0.50f, 0.00f}, {0.25f, 0.00f},
        {0.50f,    t2}, {0.25f,    t2}, {0.25f, 1.00f}, // bottom
        {0.50f,    t2}, {0.25f, 1.00f}, {0.50f, 1.00f}
    });
    // clang-format on

    setTextureCoordinates(texCoords);
}
