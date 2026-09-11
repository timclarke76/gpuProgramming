/*
 * glo/mesh/shapes/cube.h
 *
 * A cube mesh. Includes normals and texture coordinates.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <glm/glm.hpp>

#include "../mesh_impl.h"

class CubeMesh : public MeshImpl<glm::vec3>
{
    // === Radius of our cube, giving a constructed size of 1.0 x 1.0 x 1.0
    static constexpr GLfloat RADIUS = 0.5f;

  public:
    CubeMesh();
};
