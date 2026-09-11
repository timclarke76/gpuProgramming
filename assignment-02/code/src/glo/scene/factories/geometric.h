/*
 * glo/scene/factories/geometric.h
 *
 * Creates a geometric shape SceneNodes, for use in graphs. Meshes are cached
 * for reuse.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <cstddef>
#include <map>
#include <memory>
#include <string>

#include "../../mesh/buffers/colour.h"

class Mesh;
class Renderable;
class SceneNode;
class TextureFactory;

class GeometricFactory
{
  public:
    typedef std::shared_ptr<Mesh> MeshPtr;
    typedef std::shared_ptr<Renderable> RenderablePtr;
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

  public:
    // clang-format off
    GeometricFactory(const std::shared_ptr<TextureFactory> & textureFactory)
        : textureFactory(textureFactory) {}

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

    // the texture factory, set by the client during construction
    std::shared_ptr<TextureFactory> textureFactory;

    // shape to colour string to mesh map
    MeshCache meshCache;
};
