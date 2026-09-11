/*
 * glo/mesh/textures/image_2d.cpp
 *
 * © Tim Clarke November 2025
 */

#include <format>
#include <stdexcept>
#include <string>
#include <utility>

#include "../support/stb_image.h"

#include "../../log.h"

#include "image_2d.h"

/*
 * Loads an image from file, and stores it in an OpenGL texture buffer. Image
 * loading is delegated to stb_image.
 *
 * Data is stored in RGBA format, allowing the use of the alpha channel.
 *
 * filename: the full path of the file to read the image data from
 * wrapS: how to handle wrapping on the horizontal --- defaults to GL_REPEAT
 * wrapT: how to handle wrapping on the vertical --- defaults to GL_REPEAT
 * minFilter: the function OpenGL should use when it determines that the level
 *   of detail required is small enough. Defaults to GL_LINEAR
 * magFilter: the function OpenGL should use when it determines that the level
 *   of detail required is large enough. Defaults to GL_NEAREST
 */
Image2DTexture::Image2DTexture(
    const std::string_view & filename, const GLint wrapS, const GLint wrapT,
    const GLint minFilter, const GLint magFilter) : ImageTexture(GL_TEXTURE_2D)
{
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    GL_ERROR_CHECK;

    // === LOAD THE IMAGE FROM FILE ===
    int w, h, channels;
    const auto data =
        stbi_load(filename.data(), &w, &h, &channels, STBI_rgb_alpha);

    if (data == nullptr)
    {
        // unable to load the given image file
        glDeleteTextures(1, &id);
        throw std::runtime_error(
            std::string("Failed to open texture file ") + filename.data());
    }

    // === COPY THE IMAGE DATA TO OUR TEXTURE BUFFER ===
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        data);
    GL_ERROR_CHECK;

    // all data has been copied, so we no longer need to keep the data
    stbi_image_free(data);

    // === GENERATE A MIPMAP
    glGenerateMipmap(GL_TEXTURE_2D);
    GL_ERROR_CHECK;
}
