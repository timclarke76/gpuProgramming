/*
 * glo/mesh/shapes/skybox.cpp
 *
 * © Tim Clarke November 2025
 */

#include <vector>

#include "skybox.h"

/*
 * Constructs a skybox mesh, size 1x1x1.
 *
 * All eight vertex positions are calculated manually, followed by a vector of
 * indexes. The deindex method is then used to provide our final vector of
 * vertics, to be used by OpenGL.
 *
 * Wrapping is clockwise, as we are looking from the inside out.
 *
 * Normals and texture coordinates are not required for a skybox.
 */
SkyboxMesh::SkyboxMesh()
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
        0, 2, 1, 0, 3, 2, // front
        1, 7, 4, 1, 2, 7, // right
        4, 6, 5, 4, 7, 6, // back
        5, 3, 0, 5, 6, 3, // left
        3, 7, 2, 3, 6, 7, // top
        1, 5, 0, 1, 4, 5  // bottom
    });

    // And deindex the vertices into data to be used by OpenGL

    // clang-format on
    auto deindexed = deindex(vertices, indices);
    setVertices(GL_TRIANGLES, deindexed);
}
