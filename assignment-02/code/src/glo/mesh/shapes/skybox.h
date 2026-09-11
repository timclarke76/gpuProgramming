/*
 * glo/mesh/shapes/skybox.h
 *
 * A skybox mesh. Does not require normals and texture coordinates.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <glm/glm.hpp>

#include "../mesh_impl.h"

class SkyboxMesh : public MeshImpl<glm::vec3>
{
    // === Radius of our skybox, giving a constructed size of 1.0 x 1.0 x 1.0
    static constexpr GLfloat RADIUS = 0.5f;

  public:
    SkyboxMesh();
};
