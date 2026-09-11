/*
 * glo/mesh/shapes/sphere.h
 *
 * A sphere mesh. Includes indices, normals, and texture coordinates.
 *
 * CITE: I asked DeepSeek (DeepSeek 2025) to generate code for a sphere, using a
 * triangle array. The code was modified to cater for:
 *     • efficiency --- the code was cleaned up, duplication removed, and value
 *       calculations minimised (for example, y only has to be calculated once
 *       per ring).
 *     • types --- the data types were changed to use OpenGL types (for example,
 *       from float to GLfloat).
 *     • winding --- winding had to be corrected.
 *     • adaption --- the relevant code was extracted to place into the
 *       SphereMesh. Variable names were modified for consistency with the
 *       current codebase, and consts specified as appropriate.
 *
 * DeepSeek (2025). DeepSeek-V3.2-Exp. url: https://chat.deepseek.com
 *
 * My own research into OpenGL practices has led me to understand that modern
 * GPU architectures are optimised for triangle arrays. By taking advantage of
 * this, we no longer have to specify degenerate triangles, and only need to
 * make one call for the whole sphere to the GPU, instead of one call per ring.
 * This all comes at the cost of a higher number of vertices. However, in modern
 * GPUs, space is no longer the premium that it once was.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <cmath>
#include <vector>

#include <glm/glm.hpp>

#include "../mesh_impl.h"

class SphereMesh : public MeshImpl<glm::vec3>
{
    // === Radius of our sphere, giving a constructed size of 1.0 x 1.0 x 1.0
    static constexpr GLfloat RADIUS = 0.5f;

    // === Higher values require more vertices, but generate smoother spheres.
    static constexpr GLuint NUM_SEGMENTS = 64;
    static constexpr GLuint NUM_RINGS = 32;

    struct SphereData
    {
        std::vector<glm::vec3> vertices;
        std::vector<GLuint> indices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
    };

  public:
    SphereMesh(GLuint segments = NUM_SEGMENTS, GLuint rings = NUM_RINGS);

  private:
    static SphereData createSphereData(const GLuint numSegments,
        const GLuint numRings);
};
