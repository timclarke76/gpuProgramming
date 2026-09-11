/*
 * glo/mesh/buffers/colour.h
 *
 * Holds colour buffer information for an assembly vertex array object.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <cstddef>
#include <memory>

#include <glm/glm.hpp>

#include "../../colour.h"
#include "buffer_impl.h"

class ColourBuffer : public BufferImpl<glm::vec4>
{
  public:
    ColourBuffer(const Colour & colour, const GLsizei size);
    ColourBuffer(const std::shared_ptr<Colour> colour, const GLsizei size) :
        ColourBuffer(*colour, size) {}
};
