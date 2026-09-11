/*
 * glo/mesh/shapes/decal.h
 *
 * A decal mesh. Includes normals and texture coordinates.
 *
 * A decal may be used to add a texture image to another mesh, by scaling the
 * decal appropriately. This decal provides front and back faces.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <glm/glm.hpp>

#include "../mesh_impl.h"

class DecalMesh : public MeshImpl<glm::vec3>
{
    // === Radius of our decal, giving a constructed size of 1.0 x 1.0 x 1.0
    static constexpr GLfloat RADIUS = 0.5f;

  public:
    DecalMesh();
};
