/*
 * glo/mesh/shapes/pyramid.h
 *
 * A pyramid mesh. Includes normals and texture coordinates.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <glm/glm.hpp>

#include "../mesh_impl.h"

class PyramidMesh : public MeshImpl<glm::vec3>
{
    // === Radius of our pyramid, giving a constructed size of 1.0 x 1.0 x 1.0
    static constexpr GLfloat RADIUS = 0.5f;

  public:
    PyramidMesh();
};
