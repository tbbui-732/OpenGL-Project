//#include "assimp/Importer.hpp"
//#include "assimp/scene.h"
//#include "assimp/postprocess.h"

// NOTE: DELETE ONCE FINISHED. FOR LSP PURPOSES
#include <string>
#include <iostream>

#include "../assimp/Importer.hpp"
#include "../assimp/scene.h"
#include "../assimp/postprocess.h"

class Model {
private:
    std::string directory;
    std::vector<Mesh> meshes;

    void loadModel (std::string path) {
        Assimp::Importer importer;

        // Convert non-triangles to triangle primitives, and flips image around y-axis
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        // Check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ASSIMP::ERROR::" << importer.GetErrorString() << std::endl;
            exit(1);
        }

        directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode *node, const aiScene *scene) {
        // process node's meshes
        for (int i = 0; i < node->mNumMeshes; ++i) {
            int meshIdx = node->mMeshes[i];
            aiMesh *mesh = scene->mMeshes[meshIdx];
            meshes.push_back(processMesh(mesh, scene));
        }

        // recursively process node's children's meshes too
        for (int i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for (int i = 0; i < mesh->mNumVertices; ++i) {
            Vertex vertex;
            glm::vec3 pos;
            glm::vec3 norm;
            glm::vec3 texcoord;

            // process vertex positions (position, normal, texcoords)

            pos.x = mesh->mVertices[i].x;
            pos.y = mesh->mVertices[i].y;
            pos.z = mesh->mVertices[i].z;

            norm.x = mesh->mNormals[i].x;
            norm.y = mesh->mNormals[i].y;
            norm.z = mesh->mNormals[i].z;

            glm::vec2 vec(0.0f, 0.0f);
            if (mesh->mTextureCoords[0]) {
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
            }

            vertex.Position = pos;
            vertex.Normal = norm;
            vertex.TexCoords = vec;

            vertices.push_back(vertex);
        }

        // process indices
        // ...

        // process material
        if (mesh->mMaterialIndex >= 0) {
            // ...
        }

        return Mesh(vertices, indices, textures);
    }
public:
};
