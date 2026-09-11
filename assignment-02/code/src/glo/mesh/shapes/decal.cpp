/*
 * glo/mesh/shapes/decal.cpp
 *
 * © Tim Clarke November 2025
 */

#include "decal.h"

#include <vector>

/*
 * Constructs a decal mesh, size 1x1x1. It should be scaled appropriately for
 * the target mesh.
 *
 * All eight vertex positions of a cube are calculated manually, followed by a
 * vector of indexes for the front and back faces. The deindex method is then
 * used to provide our final vector of vertics, to be used by OpenGL.
 *
 * Normals are automatically calculated from the deindexed vertices, using the
 * calculateNormals method.
 *
 * Texture coordinates are calculated based on the front and back images placed
 * horizontally next to each other. These are manually specified.
 */
DecalMesh::DecalMesh()
{
    // Calculate all of our vertices. These are the same vertices as a cube
    // uses.

    // clang-format off
    const auto buffer = std::vector<glm::vec3>({
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

    // Specify how each vertex position should be used to construct ONLY the
    // front and back faces of a cube

    const auto elements = std::vector<GLuint>({
        0, 1, 2, 0, 2, 3, // front
        4, 5, 6, 4, 6, 7  // back
    });

    // And deindex the vertices into data to be used by OpenGL

    // clang-format on
    auto deindexed = deindex(buffer, elements);
    setVertices(GL_TRIANGLES, deindexed);
    setNormals(calculateNormals(deindexed));

    auto tex_coords = std::vector<glm::vec2>();
    tex_coords.insert(tex_coords.end(), {
        {0.0f, 1.0f}, {0.5f, 1.0f}, {0.5f, 0.0f}, // front
        {0.0f, 1.0f}, {0.5f, 0.0f}, {0.0f, 0.0f},
        {0.5f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, // back
        {0.5f, 1.0f}, {1.0f, 0.0f}, {0.5f, 0.0f}
    });

    setTextureCoordinates(tex_coords);
}
