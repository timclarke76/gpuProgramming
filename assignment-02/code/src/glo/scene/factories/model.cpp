/*
 * glo/scene/factories/model.cpp
 *
 * © Tim Clarke November 2025
 */

#include "model.h"

#include <iostream>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "../../colour.h"
#include "../../log.h"
#include "../../mesh/model.h"
#include "../node.h"
#include "../renderable.h"
#include "texture.h"

/*
 * Create a scene of ModelMesh objects from a model file. See
 * https://github.com/assimp/assimp/blob/master/doc/Fileformats.md for supported
 * file formats.
 *
 * If an error occurs loading the model, it is logged and an exception thrown.
 */
std::shared_ptr<SceneNode>
ModelFactory::create(const std::string & filename)
{
    Assimp::Importer importer;
    const aiScene * assimpScene = importer.ReadFile(basePath + "/" + filename,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !assimpScene->mRootNode)
    {
        std::stringstream ss;

        ss << "Error loading \"" << (basePath + "/" + filename)
           << "\": " << importer.GetErrorString() << std::endl;
        Log::e("ModelFactory", ss.str());

        throw std::runtime_error(ss.str());
    }

    return processAssimpNode(assimpScene, assimpScene->mRootNode);
}

/*
 * Translates the assimp graph to our own graph. Each aiMesh in the current
 * aiNode is used to create a Renderable and added to the returned SceneNode.
 * This method is then called recursively for each descendant.
 */
std::shared_ptr<SceneNode>
ModelFactory::processAssimpNode(const aiScene * const assimpScene,
    const aiNode * const assimpNode)
{
    auto sceneNode = std::make_shared<SceneNode>();

    for (auto meshIdx = 0; meshIdx < assimpNode->mNumMeshes; meshIdx++)
    {
        const auto assimpMesh =
            assimpScene->mMeshes[assimpNode->mMeshes[meshIdx]];
        sceneNode->addRenderable(getRenderable(assimpScene, assimpMesh));
    }

    for (auto childIdx = 0; childIdx < assimpNode->mNumChildren; childIdx++)
    {
        sceneNode->addChild(
            processAssimpNode(assimpScene, assimpNode->mChildren[childIdx]));
    }

    return sceneNode;
}

/*
 * Returns a Renderable for the given aiMesh. If the aiMesh has a texture, this
 * is loaded. Otherwise a ColourBuffer of DEFAULT_COLOUR is applied.
 *
 * TODO: currently only the first texture is loaded, which is okay for most
 * models. But some models may have multiple textures.
 */
std::shared_ptr<Renderable>
ModelFactory::getRenderable(const aiScene * const assimpScene,
    const aiMesh * const assimpMesh)
{
    std::shared_ptr<Renderable> renderable;
    auto modelMesh = std::make_shared<ModelMesh>(assimpMesh);
    auto colStr = DEFAULT_COLOUR;
    aiMaterial * mat = nullptr;

    if (assimpMesh->mMaterialIndex >= 0)
    {
        mat = assimpScene->mMaterials[assimpMesh->mMaterialIndex];

        if (mat != nullptr)
        {
            aiColor4D col(0.f, 0.f, 0.f, 0.0f);

            if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                const auto texture = getTexture(aiTextureType_DIFFUSE, mat);
                renderable = std::make_shared<Renderable>(modelMesh, texture);
            }
            else if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, col) == AI_SUCCESS)
            {
                colStr = Colour::toHex(col.r, col.g, col.b, col.a);
            }
        }
    }

    if (!renderable) // if the aiMesh does not have a texture
    {
        const auto colour = textureFactory->getColour(colStr);
        const auto colourBuffer =
            textureFactory->getColourBuffer(modelMesh->getCount(), colStr);

        modelMesh->setColours(colourBuffer);
        renderable = std::make_shared<Renderable>(modelMesh, *colour);
    }

    if ((mat != nullptr) && mat->GetTextureCount(aiTextureType_NORMALS))
    {
        const auto texture = getTexture(aiTextureType_NORMALS, mat);
        renderable->getMaterial().setNormals(texture);
    }

    return renderable;
}

/*
 * Returns an Image2DTexture for the first diffuse texture in the aiMterial. The
 * client must have confirmed the material holds at least one texture of the
 * gven type before calling this method.
 */
std::shared_ptr<Image2DTexture>
ModelFactory::getTexture(const aiTextureType textureType,
    const aiMaterial * const assimpMaterial)
{
    aiString assimpPath;
    assimpMaterial->GetTexture(textureType, 0, &assimpPath);
    return textureFactory->getImage2DTexture(assimpPath.C_Str());
}
