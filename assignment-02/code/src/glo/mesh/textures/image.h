/*
 * glo/mesh/textures/image.h
 *
 * The base class of all image textures.
 *
 * This class implements stb. No other class should #define
 * STB_IMAGE_IMPLEMENTATION.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../../error.h"

class ImageTexture
{
    protected:
    ImageTexture(const GLenum target);

  public:
    virtual ~ImageTexture();

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
        glBindTexture(target, id);
        GL_ERROR_CHECK;
    }

    void unbind() const
    {
        glBindTexture(target, 0);
        GL_ERROR_CHECK;
    }

    operator GLuint() const { return id; }

  protected:
    GLuint id = 0;

  private:
    GLenum target;
};
