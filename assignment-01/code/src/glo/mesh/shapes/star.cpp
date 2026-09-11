/*
 * glo/mesh/shapes/star.cpp
 *
 * © Tim Clarke November 2025
 */

#include "star.h"

#include <vector>

#include <glm/gtc/constants.hpp>

/*
 * Our vertices and normals are calculated in generateFace and generateEdge.
 * Data is deindexed to ensure sharp edges.
 */
StarMesh::StarMesh()
{
    auto vertices = std::vector<glm::vec3>();
    auto indices = std::vector<GLuint>();

    generateFace(RADIUS, vertices, indices);
    generateFace(-RADIUS, vertices, indices);
    generateEdge(vertices, indices);

    const auto deindexed = deindex(vertices, indices);
    setVertices(GL_TRIANGLES, deindexed);
    setNormals(calculateNormals(deindexed));
}

/*
 * Generates a front or back face, according to the value of z. The face is
 * simply a collection of triangles, each with one vertice at the centre,
 * another vertice at the tip of a point, and the third at the base between
 * points.
 */
void
StarMesh::generateFace(const GLfloat z, std::vector<glm::vec3> & vertices,
    std::vector<GLuint> & indices)
{
    // we need to save where we start to save vertices, so we can offset our
    // indices appropriately
    const auto faceStartIdx = vertices.size();

    // add the centre position
    vertices.push_back({0.0f, 0.0f, z});

    for (auto idx = 0; idx <= NUM_POINTS * 2; idx++)
    {
        // every other iteration of the loop is a point
        GLfloat radius = RADIUS;
        if (idx % 2 == 0) radius -= POINT_LENGTH;

        const GLfloat angle = glm::pi<GLfloat>() *
            static_cast<GLfloat>(idx)
            / static_cast<GLfloat>(NUM_POINTS);
        const GLfloat x = radius * cos(angle);
        const GLfloat y = radius * sin(angle);

        vertices.push_back({x, y, z});
    }

    // We now have a centre vertex, and our vertices descrbing the edge of the
    // star. Note that we check the value of z to ensure winding is counter
    // clockwise.
    for (auto idx = 0; idx < NUM_POINTS * 2; idx++)
    {
        indices.push_back(faceStartIdx);
        indices.push_back(faceStartIdx + idx + (z < 0.0f ? 2 : 1));
        indices.push_back(faceStartIdx + idx + (z < 0.0f ? 1 : 2));
    }
}

/*
 * Our edge vertices are already determined, so we just jump back-and-forth
 * between the faces to create the edge.
 */
void
StarMesh::generateEdge(std::vector<glm::vec3> & vertices,
    std::vector<GLuint> & indices)
{
    // 1 is added for all three calculations, as here we are not interested in
    // the centre vertices
    const auto FRONT_START = 1;
    const auto BACK_OFFSET = NUM_POINTS * 2 + 1;
    const auto BACK_START = FRONT_START + BACK_OFFSET + 1;

    for (auto idx = 0; idx < NUM_POINTS * 2; idx++)
    {
        const GLuint frontCurrent = FRONT_START + idx;
        const GLuint frontNext = FRONT_START + ((idx + 1) % BACK_OFFSET);
        const GLuint backCurrent = BACK_START + idx;
        const GLuint backNext = BACK_START + ((idx + 1) % BACK_OFFSET);

        // clang-format off
        indices.insert(indices.end(), {
            frontCurrent, backCurrent, frontNext,
            backCurrent, backNext, frontNext
        });
        // clang-format on
    }
}
