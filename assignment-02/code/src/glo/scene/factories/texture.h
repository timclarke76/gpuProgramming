/*
 * glo/scene/factories/texture.h
 *
 * Creates and caches colours, colour buffers, and textures.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <map>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Colour;
class ColourBuffer;
class Image2DTexture;

class TextureFactory
{
  private:
    using ColourPtr = std::shared_ptr<Colour>;
    using ColourCache = std::map<const std::string, ColourPtr>;

    using ColourBufferPtr = std::shared_ptr<ColourBuffer>;
    using ColourBufferMap = std::map<const std::string, ColourBufferPtr>;
    using ColourBufferMapPtr = std::unique_ptr<ColourBufferMap>;
    using ColourBufferCache = std::map<GLuint, ColourBufferMapPtr>;

    using Image2DTexturePtr = std::shared_ptr<Image2DTexture>;
    using TextureCache = std::map<const std::string, Image2DTexturePtr>;

  public:
    TextureFactory(const char * const basePath) : basePath(basePath) {}

    const ColourBufferPtr getColourBuffer(const GLuint size,
        const std::string & colourStr);
    const ColourPtr getColour(const std::string & colourStr);
    const Image2DTexturePtr getImage2DTexture(const std::string & filename);

  private:
    // the base path of texture files. Set by the client during construction
    const std::string basePath;

    // buffer size to colour string to colour buffer map
    ColourBufferCache colourBufferCache;

    // colour string to Colour map
    ColourCache colourCache;

    // texture file to Image2DTexture map
    TextureCache textureCache;
};
