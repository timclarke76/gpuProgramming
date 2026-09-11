/*
 * glo/mesh/textures/image.cpp
 *
 * © Tim Clarke November 2025
 */

#include <format>
#include <stdexcept>
#include <string>
#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include "../support/stb_image.h"

#include "../../log.h"

#include "image.h"

/*
 * Allocates the texture for the derived image texture class.
 *
 * target: the target for the buffer
 */
ImageTexture::ImageTexture(const GLenum target) :
    target(target)
{
    // Generate our texture id, and fail if one cannot be created
    glGenTextures(1, &id);
    if (id == 0) throw std::runtime_error("Failed to create texture");
    Log::d("ImageTexture", std::format("Texture {} created", id));
}

/*
 * Destroy the image texture. Check the id is not zero, as this may be a
 * temporary instance.
 */
ImageTexture::~ImageTexture()
{
    if (id != 0)
    {
        glDeleteTextures(1, &id);
        GL_ERROR_CHECK;
        Log::d("ImageTexture", std::format("Texture {} deleted", id));
    }
}

/*
 * Move constructor.  Take the values from other, and reset other's values
 * to ensure safe destruction.
 */
ImageTexture::ImageTexture(ImageTexture && other)
{
    if (id != 0)
    {
        glDeleteTextures(1, &id);
        GL_ERROR_CHECK;
        Log::d("ImageTexture", std::format("Texture {} deleted", id));
    }

    id = std::exchange(other.id, 0);
    target = std::exchange(other.target, 0);
}

/*
 * Move operator.  Take the values from other, and reset other's values
 * to ensure safe destruction.
 */
ImageTexture &
ImageTexture::operator=(ImageTexture && other)
{
    if (this != &other)
    {
        if (id != 0)
        {
            glDeleteTextures(1, &id);
            GL_ERROR_CHECK;
            Log::d("ImageTexture", std::format("Texture {} deleted", id));
        }

        id = std::exchange(other.id, 0);
        target = std::exchange(other.target, 0);
    }

    return *this;
}
