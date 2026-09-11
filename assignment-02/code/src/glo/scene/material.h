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
#include "../mesh/textures/image_2d.h"

class Material
{
  public:
    Material(const Colour & baseColour) : baseColour(baseColour) {}
    Material(std::shared_ptr<Image2DTexture> texture) : texture(texture) {}

    bool hasBaseColor() const { return baseColour.has_value(); }
    const std::optional<Colour> & getBaseColour() const { return baseColour; }

    // clang-format off
    std::optional<std::shared_ptr<Image2DTexture>> getTexture()
        { return texture; }
    bool hasTexture() const { return texture.has_value(); }

    Material & setNormals(std::shared_ptr<Image2DTexture> normals)
        { this->normals = normals; return *this; }
    std::optional<std::shared_ptr<Image2DTexture>> getNormals()
        { return normals; }
    bool hasNormals() const { return normals.has_value(); }
    // clang-format on

  private:
    std::optional<Colour> baseColour;
    std::optional<std::shared_ptr<Image2DTexture>> texture;
    std::optional<std::shared_ptr<Image2DTexture>> normals;
};
