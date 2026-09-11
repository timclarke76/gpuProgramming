/*
 * glo/scene/factories/texture.cpp
 *
 * © Tim Clarke November 2025
 */

#include "texture.h"

#include "../../colour.h"
#include "../../mesh/buffers/colour.h"
#include "../../mesh/textures/image_2d.h"

/*
 * Returns any cached ColourBuffer that matches the vetex size and colour
 * string. A ColourBuffer is created and cached if a previous one has not been
 * cached.
 */
const TextureFactory::ColourBufferPtr
TextureFactory::getColourBuffer(const GLuint size,
    const std::string & colourStr)
{
    if ((colourBufferCache.count(size) != 0) &&
        (colourBufferCache[size]->count(colourStr) != 0))
    {
        return colourBufferCache[size]->at(colourStr);
    }

    if (colourBufferCache.count(size) == 0)
    {
        colourBufferCache[size] = ColourBufferMapPtr(new ColourBufferMap());
    }

    colourBufferCache[size]->emplace(colourStr,
        std::make_shared<ColourBuffer>(getColour(colourStr), size));

    return colourBufferCache[size]->at(colourStr);
}

/*
 * Returns any cached Colour that matches the colour string. A Colour is created
 * and cached if a previous one has not been cached.
 */
const TextureFactory::ColourPtr
TextureFactory::getColour(const std::string & colourStr)
{
    if (colourCache.count(colourStr) != 0) return colourCache[colourStr];

    colourCache[colourStr] = std::make_shared<Colour>(colourStr);

    return colourCache[colourStr];
}

/*
 * Returns any cached texture that matches the filename. A texture is created
 * and cached if a previous one has not been cached.
 */
const TextureFactory::Image2DTexturePtr
TextureFactory::getImage2DTexture(const std::string & filename)
{
    if (textureCache.count(filename) != 0) return textureCache[filename];

    textureCache[filename] =
        std::make_shared<Image2DTexture>(basePath + "/" + filename);

    return textureCache[filename];
}
