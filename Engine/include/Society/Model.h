
#ifndef SC_MODEL_H
#define SC_MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Society/Vendor/stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Society/Mesh.h>
#include <Society/Shader.h>

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

namespace sc {
    unsigned int TextureFromFile(
        const char *path,
        const std::string &directory,
        bool gamma = false
        );

    class Model {
        public:
            // model data
            std::vector<Texture> textures_loaded{};
            // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
            std::vector<Mesh> meshes{};
            std::string directory{};
            bool gammaCorrection;

            // constructor, expects a filepath to a 3D model.
            explicit
            Model(
                std::string const &path,
                bool gamma = false
                );

            // draws the model, and thus all its meshes

            void Draw(const Shader &shader);

        private:
            // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
            void loadModel(std::string const &path);

            // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
            void processNode(const aiNode *node, const aiScene *scene);

            Mesh processMesh(const aiMesh *mesh, const aiScene *scene);

            // checks all material textures of a given type and loads the textures if they're not loaded yet.
            // the required info is returned as a Texture struct.
            std::vector<Texture> loadMaterialTextures(
                const aiMaterial *mat,
                aiTextureType type,
                std::string typeName
                );
    };

    unsigned int TextureFromFile(
        const char *path,
        const std::string &directory,
        bool gamma
        );
}

#endif

// //
// // Created by Peter Farber on 5/16/24.
// //
//
// #ifndef SC_MODEL_H
// #define SC_MODEL_H
// #include <vector>
// #include <glm/vec2.hpp>
// #include <glm/vec3.hpp>
//
// namespace sc {
//     struct Vertex {
//         // position
//         glm::vec3 Position;
//         // normal
//         glm::vec3 Normal;
//         // texCoords
//         glm::vec2 TexCoords;
//     };
//
//     class Model {
//         std::vector<Vertex> m_vertices;
//
//         unsigned int m_VBO, m_VAO, m_EBO;
//
//         public:
//             Model();
//
//             bool loadObj(const char *path);
//
//             void Generate();
//
//             void Draw() const;
//     };
// }
//
// #endif //OBJLOADER_H
