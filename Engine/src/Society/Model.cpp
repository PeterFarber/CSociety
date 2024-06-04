// //
// // Created by Peter Farber on 5/16/24.
// //
#include <Society/Model.h>

namespace sc {
    Model::Model(std::string const &path, const bool gamma):
        gammaCorrection(
            gamma
            ) {
        // stbi_set_flip_vertically_on_load(true);
        loadModel(path);
        // stbi_set_flip_vertically_on_load(false);
    }

    void Model::processNode(const aiNode *node, const aiScene *scene) {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            // the node object only contains indices to index the actual objects in the scene.
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh(const aiMesh *mesh, const aiScene *scene) {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            glm::vec3 vector;
            // we declare a placeholder std::vector  since assimp uses its own std::vector  class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals()) {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0])
            // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            } else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(
            material,
            aiTextureType_DIFFUSE,
            "texture_diffuse"
            );
        textures.insert(
            textures.end(),
            diffuseMaps.begin(),
            diffuseMaps.end()
            );
        // 2. specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(
            material,
            aiTextureType_SPECULAR,
            "texture_specular"
            );
        textures.insert(
            textures.end(),
            specularMaps.begin(),
            specularMaps.end()
            );
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(
            material,
            aiTextureType_HEIGHT,
            "texture_normal"
            );
        textures.insert(
            textures.end(),
            normalMaps.begin(),
            normalMaps.end()
            );
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(
            material,
            aiTextureType_AMBIENT,
            "texture_height"
            );
        textures.insert(
            textures.end(),
            heightMaps.begin(),
            heightMaps.end()
            );

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> Model::loadMaterialTextures(
        const aiMaterial *mat,
        const aiTextureType type,
        std::string typeName
        ) {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++) {
                if (std::strcmp(
                        textures_loaded[j].path.data(),
                        str.C_Str()
                        ) == 0) {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip) {
                // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(
                    str.C_Str(),
                    this->directory
                    );
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
                // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
            }
        }
        return textures;
    }

    unsigned int TextureFromFile(
        const char *path,
        const std::string &directory,
        bool gamma
        ) {

        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(
            filename.c_str(),
            &width,
            &height,
            &nrComponents,
            0
            );
        if (data) {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                format,
                width,
                height,
                0,
                format,
                GL_UNSIGNED_BYTE,
                data
                );
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_LINEAR
                );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        } else {
            std::cout << "Texture failed to load at path: " << path <<
                std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

    void Model::Draw(const Shader &shader) {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

    void Model::loadModel(std::string const &path) {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(
            path,
            aiProcess_Triangulate | aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs | aiProcess_CalcTangentSpace
            );
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !
            scene->
            mRootNode) // if is Not Zero
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() <<
                std::endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }
}

// #include <string>
// #include <glad/glad.h>
//
// #include <Society/Model.h>
// #include <assimp/mesh.h>
//
// namespace sc {
//     Model::Model(): m_VBO(0), m_VAO(0), m_EBO(0) {}
//
//     bool Model::loadObj(const char *path) {
//         std::std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
//         std::std::vector<glm::vec3> temp_vertices;
//         std::std::vector<glm::vec2> temp_uvs;
//         std::std::vector<glm::vec3> temp_normals;
//         FILE *file = fopen(path, "r");
//         if (file == nullptr) {
//             printf("Impossible to open the file !\n");
//             return false;
//         }
//         while (true) {
//             char lineHeader[128];
//             // read the first word of the line
//             if (fscanf(file, "%s", lineHeader) == EOF)
//                 break; // EOF = End Of File. Quit the loop.
//
//             // else : parse lineHeader
//             if (strcmp(lineHeader, "v") == 0) {
//                 glm::vec3 vertex;
//                 fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
//                 temp_vertices.push_back(vertex);
//             } else if (strcmp(lineHeader, "vt") == 0) {
//                 glm::vec2 uv;
//                 fscanf(file, "%f %f\n", &uv.x, &uv.y);
//                 temp_uvs.push_back(uv);
//             } else if (strcmp(lineHeader, "vn") == 0) {
//                 glm::vec3 normal;
//                 fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
//                 temp_normals.push_back(normal);
//             } else if (strcmp(lineHeader, "f") == 0) {
//                 std::std::string vertex1, vertex2, vertex3;
//                 unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//                 const int matches = fscanf(
//                     file,
//                     "%d/%d/%d %d/%d/%d %d/%d/%d\n",
//                     &vertexIndex[0],
//                     &uvIndex[0],
//                     &normalIndex[0],
//                     &vertexIndex[1],
//                     &uvIndex[1],
//                     &normalIndex[1],
//                     &vertexIndex[2],
//                     &uvIndex[2],
//                     &normalIndex[2]
//                 );
//                 if (matches != 9) {
//                     printf(
//                         "File can't be read by our simple parser : ( Try exporting with other options\n"
//                     );
//                     return false;
//                 }
//                 vertexIndices.push_back(vertexIndex[0]);
//                 vertexIndices.push_back(vertexIndex[1]);
//                 vertexIndices.push_back(vertexIndex[2]);
//                 uvIndices.push_back(uvIndex[0]);
//                 uvIndices.push_back(uvIndex[1]);
//                 uvIndices.push_back(uvIndex[2]);
//                 normalIndices.push_back(normalIndex[0]);
//                 normalIndices.push_back(normalIndex[1]);
//                 normalIndices.push_back(normalIndex[2]);
//             }
//             // For each vertex of each triangle
//         }
//
//         for (unsigned int vertexIndex: vertexIndices) {
//             Vertex vertex = {
//                 temp_vertices[vertexIndex - 1],
//                 temp_normals[vertexIndex - 1],
//                 temp_uvs[vertexIndex - 1]
//             };
//             m_vertices.push_back(vertex);
//         }
//
//         return true;
//     }
//
//     void Model::Generate() {
//         // create buffers/arrays
//         glGenVertexArrays(1, &m_VAO);
//         glGenBuffers(1, &m_VBO);
//         glGenBuffers(1, &m_EBO);
//
//         glBindVertexArray(m_VAO);
//         // load data into vertex buffers
//         glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//         // A great thing about structs is that their memory layout is sequential for all its items.
//         // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
//         // again translates to 3/2 floats which translates to a byte array.
//         glBufferData(
//             GL_ARRAY_BUFFER,
//             m_vertices.size() * sizeof(Vertex),
//             &m_vertices[0],
//             GL_STATIC_DRAW
//         );
//
//         // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//         // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
//
//         // set the vertex attribute pointers
//         // vertex Positions
//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(
//             0,
//             3,
//             GL_FLOAT,
//             GL_FALSE,
//             sizeof(Vertex),
//             (void *) 0
//         );
//         // vertex normals
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(
//             1,
//             3,
//             GL_FLOAT,
//             GL_FALSE,
//             sizeof(Vertex),
//             (void *) offsetof(Vertex, Normal)
//         );
//         // vertex texture coords
//         glEnableVertexAttribArray(2);
//         glVertexAttribPointer(
//             2,
//             2,
//             GL_FLOAT,
//             GL_FALSE,
//             sizeof(Vertex),
//             (void *) offsetof(Vertex, TexCoords)
//         );
//         glBindVertexArray(0);
//     }
//
//     void Model::Draw() const {
//         glBindVertexArray(this->m_VAO);
//         glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
//         glBindVertexArray(0);
//     }
// }
