//
// #include <Society/Managers/ResourceManager.h>
// #include <Society/Vendor/stb_image.h>
//
// #include <glad/glad.h>
// #include <fstream>
// #include <sstream>
// #include <iostream>
//
// namespace sc {
//     ResourceManager::ResourceManager() {
//     }
//
//     Shader *ResourceManager::LoadShader(
//         const char *vShaderFile,
//         const char *fShaderFile,
//         const char *gShaderFile,
//         const std::string &name
//         ) {
//         m_shaders[name] = loadShaderFromFile(
//             vShaderFile,
//             fShaderFile,
//             gShaderFile
//             );
//         return m_shaders[name];
//     }
//
//     Shader *ResourceManager::GetShader(const std::string &name) {
//         return m_shaders[name];
//     }
//
//     Texture2D *ResourceManager::LoadTexture(
//         const char *file,
//         const bool alpha,
//         const std::string &name
//         ) {
//         m_textures[name] = loadTextureFromFile(file, alpha);
//         return m_textures[name];
//     }
//
//     Texture2D *ResourceManager::GetTexture(const std::string &name) {
//         return m_textures[name];
//     }
//
//     void ResourceManager::Clear() {
//         // (properly) delete all shaders
//         for (const auto &[fst, snd]: m_shaders)
//             glDeleteProgram(snd->ID);
//         // (properly) delete all textures
//         for (const auto &[fst, snd]: m_textures)
//             glDeleteTextures(1, &snd->ID);
//     }
//
//     Shader *ResourceManager::loadShaderFromFile(
//         const char *vShaderFile,
//         const char *fShaderFile,
//         const char *gShaderFile
//         ) {
//         // 1. retrieve the vertex/fragment source code from filePath
//         std::string vertexCode;
//         std::string fragmentCode;
//         std::string geometryCode;
//         try {
//             // open files
//             std::ifstream vertexShaderFile(vShaderFile);
//             std::ifstream fragmentShaderFile(fShaderFile);
//             std::stringstream vShaderStream, fShaderStream;
//             // read file's buffer contents into streams
//             vShaderStream << vertexShaderFile.rdbuf();
//             fShaderStream << fragmentShaderFile.rdbuf();
//             // close file handlers
//             vertexShaderFile.close();
//             fragmentShaderFile.close();
//             // convert stream into string
//             vertexCode = vShaderStream.str();
//             fragmentCode = fShaderStream.str();
//             // if geometry shader path is present, also load a geometry shader
//             if (gShaderFile != nullptr) {
//                 std::ifstream geometryShaderFile(gShaderFile);
//                 std::stringstream gShaderStream;
//                 gShaderStream << geometryShaderFile.rdbuf();
//                 geometryShaderFile.close();
//                 geometryCode = gShaderStream.str();
//             }
//         } catch (std::exception &e) {
//             std::cout << "ERROR::SHADER: Failed to read shader files" <<
//                 std::endl;
//         }
//         const char *vShaderCode = vertexCode.c_str();
//         const char *fShaderCode = fragmentCode.c_str();
//         const char *gShaderCode = geometryCode.c_str();
//         // 2. now create shader object from source code
//         auto *shader = new Shader();
//         shader->Compile(
//             vShaderCode,
//             fShaderCode,
//             gShaderFile != nullptr ? gShaderCode : nullptr
//             );
//         return shader;
//     }
//
//     Texture2D *ResourceManager::loadTextureFromFile(
//         const char *file,
//         const bool alpha
//         ) {
//         stbi_set_flip_vertically_on_load(true);
//         // create texture object
//         auto *texture = new Texture2D();
//         if (alpha) {
//             texture->Internal_Format = GL_RGBA;
//             texture->Image_Format = GL_RGBA;
//         }
//         // load image
//         int width, height, nrChannels;
//         unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
//         // now generate texture
//         texture->Generate(width, height, data);
//         // and finally free image data
//         stbi_image_free(data);
//         return texture;
//     }
// }
