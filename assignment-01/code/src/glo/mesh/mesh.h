/*
 * glo/mesh/mesh.h
 *
 * An abstract mesh class, allowing a class to interface with generic
 * mesh implmentations, without knowing about the template design of the
 * MeshImpl class.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <GL/glew.h>

#include "../scene/factories/geometric.h"
#include "buffers/colour.h"

class Mesh
{
  public:
    virtual ~Mesh() {}

    // returns the number of vertices in this mesh
    GLsizei getCount() const { return count; }

    // sets the ColourBuffer for this mesh, providing the base colour
    virtual void setColours(GeometricFactory::ColourBufferPtr buffer) = 0;

    // draws the mesh to screen
    virtual void draw() const = 0;

  protected:
    // the number of vertices in this Mesh
    GLsizei count = 0;
};
