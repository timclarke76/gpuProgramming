/*
 * glo/mesh/shapes/quad.h
 *
 * A quad mesh suitable for rendering to a framebuffer. Includes texture
 * coordinates.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <glm/glm.hpp>

#include "../mesh_impl.h"

class QuadMesh : public MeshImpl<glm::vec2>
{
    // === Radius of our quad, suitable for rendering to a framebuffer
    static constexpr GLfloat RADIUS = 1.0f;

  public:
    QuadMesh();
};
