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
public:
};
