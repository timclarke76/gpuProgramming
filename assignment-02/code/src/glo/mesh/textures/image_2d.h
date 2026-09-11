/*
 * glo/mesh/textures/image_2d.h
 *
 * Creates a 2D image texture from an image file.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <string_view>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../../error.h"
#include "image.h"

class Image2DTexture : public ImageTexture
{
  public:
    Image2DTexture(const std::string_view & filename,
        const GLint wrapS = GL_REPEAT, const GLint wrapT = GL_REPEAT,
        const GLint minFilter = GL_LINEAR, const GLint magFilter = GL_NEAREST);
};
