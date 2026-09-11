/*
 * glo/scene/factories/model.h
 *
 * Creates a ModelMesh SceneNode graph for assimp compatible file formats.
 *
 * See https://github.com/assimp/assimp/blob/master/doc/Fileformats.md for a
 * list of all supported formats.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <memory>
#include <string>

#include <assimp/material.h>

class Image2DTexture;
class Renderable;
class SceneNode;
class TextureFactory;

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;

class ModelFactory
{
  private:
    static constexpr std::string DEFAULT_COLOUR = "FFF";

  public:
    // clang-format off
    ModelFactory(const char * const basePath,
        const std::shared_ptr<TextureFactory> & textureFactory) :
        basePath(basePath), textureFactory(textureFactory) {}
    // clang-format on

    std::shared_ptr<SceneNode> create(const std::string & filename);

  private:
    std::shared_ptr<SceneNode> processAssimpNode(
        const aiScene * const assimpScene, const aiNode * const node);
    std::shared_ptr<Renderable> getRenderable(const aiScene * const assimpScene,
        const aiMesh * const assimpMesh);
    std::shared_ptr<Image2DTexture> getTexture(const aiTextureType textureType,
        const aiMaterial * const assimpMaterial);

    // the texture factory, set by the client during construction
    std::shared_ptr<TextureFactory> textureFactory;

    // the base path of model files. Set by the client during construction
    const std::string basePath;
};
