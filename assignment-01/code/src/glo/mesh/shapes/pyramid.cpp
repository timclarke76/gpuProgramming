/*
 * glo/mesh/shapes/pyramid.cpp
 *
 * © Tim Clarke November 2025
 */

#include <vector>

#include "pyramid.h"

/*
 * Constructs a pyramid mesh, size 1x1x1.
 *
 * All five vertex positions are calculated manually, followed by a vector of
 * indexes. The deindex method is then used to provide our final vector of
 * vertics, to be used by OpenGL.
 *
 * Normals are automatically calculated from the deindexed vertices, using the
 * calculateNormals method.
 *
 * Texture coordinates are calculated based on the industry stanard pyramid wrap
 * shape. These are manually specified.
 */
PyramidMesh::PyramidMesh()
{
    // Calculate all of our vertices

    // clang-format off
    const auto vertices = std::vector<glm::vec3>({
        // base
        {-RADIUS, -RADIUS,  RADIUS},
        { RADIUS, -RADIUS,  RADIUS},
        { RADIUS, -RADIUS, -RADIUS},
        {-RADIUS, -RADIUS, -RADIUS},
        
        // apex
        { 0.0f,  RADIUS,  0.0f}
    });

    // Specify how each vertex position should be used to construct a pyramid

    const auto indices = std::vector<GLuint>({
        0, 1, 4, // front
        1, 2, 4, // right
        2, 3, 4, // back
        3, 0, 4, // left
        0, 2, 1, 0, 3, 2, // base
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
    texCoords.insert( texCoords.end(), {
        {  t1,   t1}, {  t2,   t1}, {0.5f, 0.0f}, // front
        {  t2,   t1}, {  t2,   t2}, {1.0f, 0.5f}, // right
        {  t2,   t2}, {  t1,   t2}, {0.5f, 1.0f}, // back
        {  t1,   t2}, {  t1,   t1}, {0.0f, 0.5f}, // left
        {  t1,   t1}, {  t2,   t2}, {  t2,   t1}, // base
        {  t1,   t1}, {  t1,   t2}, {  t2,   t2},
    });
    // clang-format on

    setTextureCoordinates(texCoords);
}
