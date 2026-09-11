/*
 * glo/scene/renderable.h
 *
 * Renderables are container objects for a mesh and a material. They also
 * provide access to the mesh's draw method.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include "../mesh/mesh.h"
#include "material.h"

class Renderable
{
  public:
    // clang-format off
    Renderable(const std::shared_ptr<Mesh> mesh, const Material & material) :
        mesh(mesh), material(material) {}
    // clang-format on

    // clang-format off
    Renderable & render() { mesh->draw(); return *this; }
    // clang-format on

    const Material & getMaterial() const { return material; }

  private:
    const std::shared_ptr<Mesh> mesh;
    const Material material;
};
