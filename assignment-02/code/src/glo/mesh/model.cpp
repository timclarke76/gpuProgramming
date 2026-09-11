/*
 * glo/mesh/model.cpp
 *
 * © Tim Clarke November 2025
 */

#include "model.h"

#include <vector>

#include <assimp/scene.h>

ModelMesh::ModelMesh(const aiMesh * const assimpMesh)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    glm::vec3 vector;
    glm::vec2 coord;

    vertices.reserve(assimpMesh->mNumVertices);
    if (assimpMesh->HasNormals()) normals.reserve(assimpMesh->mNumVertices);

    if (assimpMesh->mTextureCoords[0])
    {
        texCoords.reserve(assimpMesh->mNumVertices);
    }

    for (auto vertexIdx = 0; vertexIdx < assimpMesh->mNumVertices; vertexIdx++)
    {
        const auto & assimpVertex = assimpMesh->mVertices[vertexIdx];
        vector.x = assimpVertex.x;
        vector.y = assimpVertex.y;
        vector.z = assimpVertex.z;
        vertices.push_back(vector);

        if (assimpMesh->HasNormals())
        {
            const auto & assimpNormal = assimpMesh->mNormals[vertexIdx];
            vector.x = assimpNormal.x;
            vector.y = assimpNormal.y;
            vector.z = assimpNormal.z;
            normals.push_back(vector);
        }

        if (assimpMesh->mTextureCoords[0])
        {
            const auto assimpCoords = assimpMesh->mTextureCoords[0][vertexIdx];
            coord.s = assimpCoords.x;
            coord.t = assimpCoords.y;
            texCoords.push_back(coord);
        }

        if (assimpMesh->HasTangentsAndBitangents())
        {
            const auto tangent = assimpMesh->mTangents[vertexIdx];
            tangents.emplace_back(tangent.x, tangent.y, tangent.z);

            const auto bitangent = assimpMesh->mBitangents[vertexIdx];
            bitangents.emplace_back(bitangent.x, bitangent.y, bitangent.z);
        }
    }

    setVertices(GL_TRIANGLES, vertices);

    if (assimpMesh->HasNormals()) setNormals(normals);
    else setNormals(calculateNormals(vertices));

    if (!texCoords.empty()) setTextureCoordinates(texCoords);

    if (assimpMesh->HasFaces())
    {
        std::vector<GLuint> indices;
        indices.reserve(assimpMesh->mNumFaces * 3);

        for (auto faceIdx = 0; faceIdx < assimpMesh->mNumFaces; faceIdx++)
        {
            const auto face = assimpMesh->mFaces[faceIdx];

            for (auto indexIdx = 0; indexIdx < face.mNumIndices; indexIdx++)
            {
                indices.push_back(face.mIndices[indexIdx]);
            }
        }

        setIndices(indices);
    }

    if (!tangents.empty())
    {
        setTangents(tangents);
        setBitangents(bitangents);
    }
}
