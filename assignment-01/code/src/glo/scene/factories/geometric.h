/*
 * glo/scene/factories/geometric.h
 *
 * Creates a geometric shape SceneNodes, for use in graphs. Meshes, colour
 * buffers, colours, and textures, are all cached for reuse.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <cstddef>
#include <map>
#include <memory>
#include <string>

#include "../../mesh/buffers/colour.h"

class ImageTexture;
class Mesh;
class Renderable;
class SceneNode;

class GeometricFactory
{
  public:
    typedef std::shared_ptr<Colour> ColourPtr;
    typedef std::shared_ptr<ColourBuffer> ColourBufferPtr;
    typedef std::shared_ptr<Mesh> MeshPtr;
    typedef std::shared_ptr<Renderable> RenderablePtr;
    typedef std::shared_ptr<ImageTexture> ImageTexturePtr;
    typedef std::shared_ptr<SceneNode> SceneNodePtr;

  private:
    enum class Shape
    {
        cube,
        cylinder,
        decal,
        pyramid,
        sphere,
        star,
    };

    using MeshMap = std::map<const std::string, MeshPtr>;
    using MeshMapPtr = std::unique_ptr<MeshMap>;
    using MeshCache = std::map<Shape, MeshMapPtr>;

    using ColourBufferMap = std::map<const std::string, ColourBufferPtr>;
    using ColourBufferMapPtr = std::unique_ptr<ColourBufferMap>;
    using ColourBufferCache = std::map<GLuint, ColourBufferMapPtr>;

    using ColourCache = std::map<const std::string, ColourPtr>;

    using TextureCache = std::map<const std::string, ImageTexturePtr>;

  public:
    GeometricFactory(const char * const basePath) : basePath(basePath) {}

    // clang-format off
    const SceneNodePtr getCube(const std::string & finish = "")
        { return getSceneNode(Shape::cube, finish); }
    const SceneNodePtr getCylinder(const std::string & finish = "")
        { return getSceneNode(Shape::cylinder, finish); }
    const SceneNodePtr getDecal(const std::string & finish = "")
        { return getSceneNode(Shape::decal, finish); }
    const SceneNodePtr getPyramid(const std::string & finish = "")
        { return getSceneNode(Shape::pyramid, finish); }
    const SceneNodePtr getSphere(const std::string & finish = "")
        { return getSceneNode(Shape::sphere, finish); }
    const SceneNodePtr getStar(const std::string & finish = "")
        { return getSceneNode(Shape::star, finish); }
    // clang-format on

  private:
    const SceneNodePtr getSceneNode(const Shape shape,
        const std::string & finish);
    const RenderablePtr getRenderable(const Shape shape,
        const std::string & finish);
    const MeshPtr getMesh(const Shape shape,
        const std::string & colourStr = "");
    const ColourBufferPtr getColourBuffer(const GLuint size,
        const std::string & colourStr);
    const ColourPtr getColour(const std::string & colourStr);
    const ImageTexturePtr getImageTexture(const std::string & filename);

    // the base path of texture files. Set by the client during construction
    const std::string basePath;

    // shape to colour string to mesh map
    MeshCache meshCache;

    // buffer size to colour string to colour buffer map
    ColourBufferCache colourBufferCache;

    // colour string to Colour map
    ColourCache colourCache;

    // texture file to ImageTexture map
    TextureCache textureCache;
};
