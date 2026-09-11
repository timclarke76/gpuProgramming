/*
 * glo/mesh/shapes/cylinder.h
 *
 * A cylinder mesh. Includes indices, normals, and texture coordinates.
 *
 * CITE: Although I have written this class myself, to aid in improving my usage
 * of sin and cos, I did at times refer to how SphereMesh and StarMesh work,
 * which themselves are modifications of DeepSeek (DeepSeek 2025) generated
 * code.
 *
 * DeepSeek (2025). DeepSeek-V3.2-Exp. url: https://chat.deepseek.com
 *
 * For why triangle arrays are used instead of strips and fans, see
 * shapes/sphere.h
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../mesh_impl.h"

class CylinderMesh : public MeshImpl<glm::vec3>
{
    // === Radius of our cylinder, giving a constructed size of 1.0 x 1.0 x 1.0
    static constexpr GLfloat RADIUS = 0.5f;

    // === Higher values require more vertices, but generate smoother edges.
    static constexpr GLuint NUM_SEGMENTS = 64;

  public:
    CylinderMesh(const GLuint numSegments = NUM_SEGMENTS);

  private:
    void generateEdge(const GLuint numSegments,
        std::vector<glm::vec3> & vertices, std::vector<GLuint> & indices,
        std::vector<glm::vec3> & normals, std::vector<glm::vec2> & texCoords);
    void generateEnd(const GLfloat y, const GLuint numSegments,
        std::vector<glm::vec3> & vertices, std::vector<GLuint> & indices,
        std::vector<glm::vec3> & normals, std::vector<glm::vec2> & texCoords);
};
