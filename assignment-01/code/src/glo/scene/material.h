/*
 * glo/scene/material.h
 *
 * Materials are container objects for either a texture, or a base colour.
 *
 * TODO: allow materials to contain both texture ans base colours, for textures
 * which contain transparent pixels.
 *
 * TODO: add values such as shininess etc.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <memory>

#include "../colour.h"
#include "../mesh/textures/image.h"

class Material
{
  public:
    Material(const Colour & baseColour) : baseColour(baseColour) {}
    Material(std::shared_ptr<ImageTexture> texture) : texture(texture) {}

    bool hasBaseColor() const { return baseColour.has_value(); }
    const std::optional<Colour> & getBaseColour() const { return baseColour; }

    bool hasTexture() const { return texture.has_value(); }

    // clang-format off
    std::optional<std::shared_ptr<ImageTexture>> getTexture()
        { return texture; }
    // clang-format on

  private:
    std::optional<Colour> baseColour;
    std::optional<std::shared_ptr<ImageTexture>> texture;
};
