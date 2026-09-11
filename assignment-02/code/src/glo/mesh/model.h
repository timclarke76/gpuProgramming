/*
 * glo/mesh/model.h
 *
 * Creates a mesh from an assimp aiMesh.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <glm/glm.hpp>

#include "mesh_impl.h"

struct aiMesh;

class ModelMesh : public MeshImpl<glm::vec3>
{
  public:
    ModelMesh(const aiMesh * const assimpMesh);
};
