/*
 * glo/scene/factories/geometric.cpp
 *
 * © Tim Clarke November 2025
 */

#include "geometric.h"

#include <memory>
#include <optional>
#include <stdexcept>

#include "../../mesh/mesh.h"
#include "../../mesh/shapes/cube.h"
#include "../../mesh/shapes/cylinder.h"
#include "../../mesh/shapes/decal.h"
#include "../../mesh/shapes/pyramid.h"
#include "../../mesh/shapes/sphere.h"
#include "../../mesh/shapes/star.h"
#include "../../mesh/textures/image.h"
#include "../node.h"
#include "../renderable.h"
#include "texture.h"

/*
 * Creates a Renderable and wraps it in a SceneNodePtr, suitable for graph
 * construction.
 */
const GeometricFactory::SceneNodePtr
GeometricFactory::getSceneNode(const Shape shape, const std::string & finish)
{
    return std::make_shared<SceneNode>(getRenderable(shape, finish));
}

/*
 * Creates and returns a renderable for the given shape and finish. As
 * Renderables are stateful, these are not cached.
 *
 * If finish starts with a # character, it is assumed to be a colour string.
 * Otherwise, it is considered a texture filename.
 */
const GeometricFactory::RenderablePtr
GeometricFactory::getRenderable(const Shape shape, const std::string & finish)
{
    if (finish[0] == '#')
    {
        return std::make_shared<Renderable>(getMesh(shape, finish),
            *textureFactory->getColour(finish));
    }
    else
    {
        return std::make_shared<Renderable>(getMesh(shape),
            textureFactory->getImage2DTexture(finish));
    }
}

/*
 * Returns any cached Mesh that matches the shape and colour string. A Mesh is
 * created and cached if a previous one has not been cached.
 */
const GeometricFactory::MeshPtr
GeometricFactory::getMesh(const Shape shape, const std::string & colourStr)
{
    if ((meshCache.count(shape) != 0) &&
        (meshCache[shape]->count(colourStr) != 0))
    {
        return meshCache[shape]->at(colourStr);
    }

    MeshPtr mesh;

    switch (shape)
    {
        // clang-format off
        case Shape::cube: mesh = std::make_shared<CubeMesh>(); break;
        case Shape::cylinder: mesh = std::make_shared<CylinderMesh>(); break;
        case Shape::decal: mesh = std::make_shared<DecalMesh>(); break;
        case Shape::pyramid: mesh = std::make_shared<PyramidMesh>(); break;
        case Shape::sphere: mesh = std::make_shared<SphereMesh>(); break;
        case Shape::star: mesh = std::make_shared<StarMesh>(); break;
        default: throw std::runtime_error("Unknown shape type");
        // clang-format on
    }

    if (colourStr.length() == 0)
    {
        mesh->setColours(
            textureFactory->getColourBuffer(mesh->getCount(), "#0000"));
    }
    else
    {
        mesh->setColours(
            textureFactory->getColourBuffer(mesh->getCount(), colourStr));
    }

    if (meshCache.count(shape) == 0)
    {
        meshCache[shape] = std::make_unique<MeshMap>();
    }

    meshCache[shape]->emplace(colourStr, std::move(mesh));

    return meshCache[shape]->at(colourStr);
}
