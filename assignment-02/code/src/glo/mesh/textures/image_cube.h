/*
 * glo/mesh/textures/image_cube.h
 *
 * Creates a cube image texture from six image files.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../../error.h"
#include "image.h"

class ImageCubeTexture : public ImageTexture
{
  public:
    ImageCubeTexture(const std::string & path,
        const std::string & extension = "jpg", const GLint wrapR = GL_REPEAT,
        const GLint wrapS = GL_REPEAT, const GLint wrapT = GL_REPEAT,
        const GLint minFilter = GL_LINEAR, const GLint magFilter = GL_LINEAR);
};
