/*
 * glo/mesh/textures/image.h
 *
 * Creates a 2D image texture from an image file.
 *
 * Uses stb_image for image loading. This class implements stb. No other class
 * should #define STB_IMAGE_IMPLEMENTATION.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <string_view>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../../error.h"

class ImageTexture
{
  public:
    ImageTexture(const std::string_view & filename,
        const GLint wrapS = GL_REPEAT, const GLint wrapT = GL_REPEAT,
        const GLint minFilter = GL_LINEAR, const GLint magFilter = GL_NEAREST);

    ~ImageTexture();

    // no copies
    ImageTexture(const ImageTexture &) = delete;
    ImageTexture & operator=(const ImageTexture &) = delete;

    // safe moves
    ImageTexture(ImageTexture && other);
    ImageTexture & operator=(ImageTexture && other);

    // bind this texture to the given texture position
    void bind(const GLenum pos) const
    {
        glActiveTexture(pos);
        glBindTexture(GL_TEXTURE_2D, id);
        GL_ERROR_CHECK;
    }

    void unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        GL_ERROR_CHECK;
    }

    operator GLuint() const { return id; }

  private:
    GLuint id = 0;
};
