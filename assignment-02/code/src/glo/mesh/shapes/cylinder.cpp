/*
 * glo/mesh/shapes/cylinder.cpp
 *
 * © Tim Clarke November 2025
 */

#include "cylinder.h"

#include <utility>

#include <glm/gtc/constants.hpp>

/*
 * Our vertices, indices, normals, and texture coordinates are all calculated in
 * generateEdge and the two calls to generateEnd. We do not deindex the data,
 * ensuring a smooth surface for our edge. To allow a sharp edge at the ends,
 * generateEnd does not reuse the vertices from generateEdge.
 */
CylinderMesh::CylinderMesh(const GLuint numSegments)
{
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;

    generateEdge(numSegments, vertices, indices, normals, texCoords);
    generateEnd( RADIUS, numSegments, vertices, indices, normals, texCoords);
    generateEnd(-RADIUS, numSegments, vertices, indices, normals, texCoords);

    setVertices(GL_TRIANGLES, vertices);
    setIndices(indices);
    setNormals(normals);
    setTextureCoordinates(texCoords);
}

/*
 * The edge of the cylinder is simply a series of triangles, alternating
 * vertically in direction. numSegements determines the number of triangles to
 * generate for the edge --- the higher the number, the smoother the edge, but
 * at a cost of more vertices.
 */
void
CylinderMesh::generateEdge(const GLuint numSegments,
    std::vector<glm::vec3> & vertices, std::vector<GLuint> & indices,
    std::vector<glm::vec3> & normals, std::vector<glm::vec2> & texCoords)
{
    const GLfloat CIRCUMFERENCE_RADIANS = 2.0f * glm::pi<GLfloat>();

    for (GLuint segIdx = 0; segIdx <= numSegments; segIdx++)
    {
        const GLfloat progress =
            (static_cast<GLfloat>(segIdx) / static_cast<GLfloat>(numSegments));
        const GLfloat angle = CIRCUMFERENCE_RADIANS * progress;
        const GLfloat x = RADIUS * std::cos(angle);
        const GLfloat z = RADIUS * std::sin(angle);

        // bottom vertices in the even indexes
        vertices.push_back({x, -RADIUS, z});

        // top vertices in the adjacent odd indexes
        vertices.push_back({x, RADIUS, z});

        // normals radiate straight from the centre
        normals.push_back({x, 0, z});
        normals.push_back({x, 0, z});

        // the bottom half of the texture for the edge
        texCoords.push_back(glm::vec2((1.0f - progress), 1.0f));
        texCoords.push_back(glm::vec2((1.0f - progress), 0.5f));
    }

    for (GLuint segIdx = 0; segIdx < numSegments; segIdx++)
    {
        // bottom vertices in the even indexes
        GLuint bottomLeft = segIdx * 2;
        GLuint bottomRight = (segIdx + 1) * 2;

        // top vertices in the adjacent odd indexes
        GLuint topLeft = bottomLeft + 1;
        GLuint topRight = bottomRight + 1;

        // clang-format off
        // clockwise, as we're looking from the outside-in
        indices.insert(indices.end(), {
            bottomLeft, topRight, bottomRight,
            bottomLeft, topLeft, topRight,
        });
        // clang-format on
    }
}

/*
 * Each end is simply a circle, created by a series of triangles. The number of
 * triangles is determined by numSegments. We do not reuse the vertices
 * from generateEdge, as doing so would prevent us from have a sharp edge.
 *
 * The y argument indicates which end is to be generated, allowing correct
 * texture coordinates, and normals, to be generated.
 *
 * For why a fan was not used, see shapes/sphere.h
 */
void
CylinderMesh::generateEnd(const GLfloat y, const GLuint numSegments,
    std::vector<glm::vec3> & vertices, std::vector<GLuint> & indices,
    std::vector<glm::vec3> & normals, std::vector<glm::vec2> & texCoords)
{
    const unsigned int startIdx = vertices.size();
    const GLfloat yNormal = y > 0.0f ? 1.0f : -1.0f;

    vertices.push_back(glm::vec3(0, y, 0));
    normals.push_back(glm::vec3(0, yNormal, 0));
    const GLfloat uOffset = y > 0.0f ? 0.0f : 0.5f;
    texCoords.push_back(glm::vec2(uOffset + 0.25f, 0.25f));

    const GLfloat CIRCUMFERENCE_RADIANS = 2.0f * glm::pi<GLfloat>();

    for (GLuint segIdx = 0; segIdx <= numSegments; segIdx++)
    {
        const GLfloat progress =
            (static_cast<GLfloat>(segIdx) / static_cast<GLfloat>(numSegments));
        const GLfloat angle = CIRCUMFERENCE_RADIANS * progress;
        const GLfloat x = RADIUS * std::cos(angle);
        const GLfloat z = RADIUS * std::sin(angle);

        vertices.push_back(glm::vec3(x, y, z));
        normals.push_back(glm::vec3(0, yNormal, 0));

        if (y > 0.0f)
        {
            texCoords.push_back(glm::vec2(0.25f + 0.25f * cos(angle),
                0.25f + 0.25f * sin(angle)));
        }
        else
        {
            texCoords.push_back(glm::vec2(0.75f + 0.25f * cos(angle),
                0.25f - 0.25f * sin(angle)));
        }
    }

    GLuint yAdd = 2, zAdd = 1;
    if (y < 0) std::swap(yAdd, zAdd);

    for (GLuint segIdx = 0; segIdx < numSegments; segIdx++)
    {
        indices.insert(indices.end(),
            {startIdx, (startIdx + segIdx + yAdd), (startIdx + segIdx + zAdd)});
    }
}
