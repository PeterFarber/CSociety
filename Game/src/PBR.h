//
// Created by Peter Farber on 5/25/24.
//

#ifndef PBR_H
#define PBR_H
#include <map>

#include "Society/Camera.h"
#include "Society/Engine.h"
#include "Society/Model.h"
#include "Society/Core/Window.h"
#include "Society/Meshes/Cube.h"
#include "Society/Meshes/Quad.h"

inline glm::vec3 lightPositions[] = {
    glm::vec3(-10.0f, 10.0f, 10.0f),
    glm::vec3(10.0f, 10.0f, 10.0f),
    glm::vec3(-10.0f, -10.0f, 10.0f),
    glm::vec3(10.0f, -10.0f, 10.0f),
};
inline glm::vec3 lightColors[] = {
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f)
};

const std::string RESOURCE_LOCATION =
    "/Users/peterfarber/CLionProjects/Society/Game/resources/";

class PBR {

    public:
        sc::Shader *m_pbrShader;

    private:
        sc::Cube m_cube;

        sc::Shader *m_equirectangularToCubemapShader;
        sc::Shader *m_irradianceShader;
        sc::Shader *m_prefilterShader;
        sc::Shader *m_brdfShader;
        sc::Shader *m_backgroundShader;

        unsigned int m_hdrTexture;
        unsigned int m_envCubemap;
        unsigned int m_captureFBO;
        unsigned int m_captureRBO;
        unsigned int m_irradianceMap;
        unsigned int m_prefilterMap;
        unsigned int m_brdfLUTTexture;

        unsigned int m_AlbedoMap;
        unsigned int m_NormalMap;
        unsigned int m_MetallicMap;
        unsigned int m_RoughnessMap;
        unsigned int m_AOMap;


        glm::mat4 captureProjection;

        std::vector<glm::mat4> captureViews;
        // std::map<std::string, unsigned int> m_textures;

    public:
        PBR(const std::string &hdrMap) {

            m_pbrShader = nullptr;
            m_equirectangularToCubemapShader = nullptr;
            m_irradianceShader = nullptr;
            m_prefilterShader = nullptr;
            m_brdfShader = nullptr;
            m_backgroundShader = nullptr;
            std::string text_loc = RESOURCE_LOCATION + "textures/metal/";
            m_AlbedoMap = loadTexture(
                (text_loc + "albedo.png").c_str());
            m_NormalMap = loadTexture(
                (text_loc + "normal.png").c_str());
            m_MetallicMap = loadTexture(
                (text_loc + "metallic.png").c_str());
            m_RoughnessMap = loadTexture(
                (text_loc + "roughness.png").c_str());
            m_AOMap = loadTexture((text_loc + "ao.png").c_str());

            std::string shaderPath = RESOURCE_LOCATION + "shaders/pbr/";

            m_pbrShader = new sc::Shader(
                (shaderPath + "pbr.vert").c_str(),
                (shaderPath + "pbr.frag").c_str());
            m_equirectangularToCubemapShader = new sc::Shader(
                (shaderPath + "cubemap.vert").c_str(),
                (shaderPath + "equirectangular_to_cubemap.frag").c_str());
            m_irradianceShader = new sc::Shader(
                (shaderPath + "cubemap.vert").c_str(),
                (shaderPath + "irradiance_convolution.frag").c_str());
            m_prefilterShader = new sc::Shader(
                (shaderPath + "cubemap.vert").c_str(),
                (shaderPath + "prefilter.frag").c_str());
            m_brdfShader = new sc::Shader(
                (shaderPath + "brdf.vert").c_str(),
                (shaderPath + "brdf.frag").c_str());
            m_backgroundShader = new sc::Shader(
                (shaderPath + "background.vert").c_str(),
                (shaderPath + "background.frag").c_str());

            m_pbrShader->use();
            m_pbrShader->setInt("irradianceMap", 0);
            m_pbrShader->setInt("prefilterMap", 1);
            m_pbrShader->setInt("brdfLUT", 2);
            m_pbrShader->setInt("albedoMap", 3);
            m_pbrShader->setInt("normalMap", 4);
            m_pbrShader->setInt("metallicMap", 5);
            m_pbrShader->setInt("roughnessMap", 6);
            m_pbrShader->setInt("aoMap", 7);

            m_backgroundShader->use();
            m_backgroundShader->setInt("environmentMap", 0);

            // pbr: setup framebuffer
            // ----------------------
            glGenFramebuffers(1, &m_captureFBO);
            glGenRenderbuffers(1, &m_captureRBO);

            glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);
            glRenderbufferStorage(
                GL_RENDERBUFFER,
                GL_DEPTH_COMPONENT24,
                512,
                512);
            glFramebufferRenderbuffer(
                GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT,
                GL_RENDERBUFFER,
                m_captureRBO);

            // pbr: load the HDR environment map
            // ---------------------------------
            stbi_set_flip_vertically_on_load(true);
            int width, height, nrComponents;
            float *data = stbi_loadf(
                (RESOURCE_LOCATION + "textures/rostock.hdr").c_str(),
                &width,
                &height,
                &nrComponents,
                0);
            // unsigned int hdrTexture;
            if (data) {
                glGenTextures(1, &m_hdrTexture);
                glBindTexture(GL_TEXTURE_2D, m_hdrTexture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RGB16F,
                    width,
                    height,
                    0,
                    GL_RGB,
                    GL_FLOAT,
                    data);
                // note how we specify the texture's data value to be float

                glTexParameteri(
                    GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);
                glTexParameteri(
                    GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T,
                    GL_CLAMP_TO_EDGE);
                glTexParameteri(
                    GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);
                glTexParameteri(
                    GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);

                stbi_image_free(data);
            } else {
                std::cout << "Failed to load HDR image." << std::endl;
            }

            // pbr: setup cubemap to render to and attach to framebuffer
            // ---------------------------------------------------------
            glGenTextures(1, &m_envCubemap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemap);
            for (unsigned int i = 0; i < 6; ++i) {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_RGB16F,
                    512,
                    512,
                    0,
                    GL_RGB,
                    GL_FLOAT,
                    nullptr);
            }
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_WRAP_S,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_WRAP_T,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_WRAP_R,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_LINEAR);
            // enable pre-filter mipmap sampling (combatting visible dots artifact)
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_MAG_FILTER,
                GL_LINEAR);

            // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
            // ----------------------------------------------------------------------------------------------
            glm::mat4 captureProjection = glm::perspective(
                glm::radians(90.0f),
                1.0f,
                0.1f,
                10.0f);
            glm::mat4 captureViews[] =
            {
                glm::lookAt(
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(-1.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, 1.0f)),
                glm::lookAt(
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, -1.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, -1.0f)),
                glm::lookAt(
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, 1.0f),
                    glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, -1.0f),
                    glm::vec3(0.0f, -1.0f, 0.0f))
            };

            // pbr: convert HDR equirectangular environment map to cubemap equivalent
            // ----------------------------------------------------------------------
            m_equirectangularToCubemapShader->use();
            m_equirectangularToCubemapShader->setInt("equirectangularMap", 0);
            m_equirectangularToCubemapShader->setMat4(
                "projection",
                captureProjection);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_hdrTexture);

            glViewport(0, 0, 512, 512);
            // don't forget to configure the viewport to the capture dimensions.
            glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
            for (unsigned int i = 0; i < 6; ++i) {
                m_equirectangularToCubemapShader->setMat4(
                    "view",
                    captureViews[i]);
                glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0,
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    m_envCubemap,
                    0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                m_cube.Draw();
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemap);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

            // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
            // --------------------------------------------------------------------------------
            glGenTextures(1, &m_irradianceMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);
            for (unsigned int i = 0; i < 6; ++i) {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_RGB16F,
                    32,
                    32,
                    0,
                    GL_RGB,
                    GL_FLOAT,
                    nullptr);
            }
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_WRAP_S,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_WRAP_T,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_WRAP_R,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_MIN_FILTER,
                GL_LINEAR);
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_MAG_FILTER,
                GL_LINEAR);

            glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);
            glRenderbufferStorage(
                GL_RENDERBUFFER,
                GL_DEPTH_COMPONENT24,
                32,
                32);

            // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
            // -----------------------------------------------------------------------------
            m_irradianceShader->use();
            m_irradianceShader->setInt("environmentMap", 0);
            m_irradianceShader->setMat4("projection", captureProjection);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemap);

            glViewport(0, 0, 32, 32);
            // don't forget to configure the viewport to the capture dimensions.
            glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
            for (unsigned int i = 0; i < 6; ++i) {
                m_irradianceShader->setMat4("view", captureViews[i]);
                glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0,
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    m_irradianceMap,
                    0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                m_cube.Draw();
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
            // --------------------------------------------------------------------------------
            glGenTextures(1, &m_prefilterMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMap);
            for (unsigned int i = 0; i < 6; ++i) {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_RGB16F,
                    128,
                    128,
                    0,
                    GL_RGB,
                    GL_FLOAT,
                    nullptr);
            }
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_WRAP_S,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_WRAP_T,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_WRAP_R,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_LINEAR);
            // be sure to set minification filter to mip_linear 
            glTexParameteri(
                GL_TEXTURE_CUBE_MAP,
                GL_TEXTURE_MAG_FILTER,
                GL_LINEAR);
            // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

            // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
            // ----------------------------------------------------------------------------------------------------
            m_prefilterShader->use();
            m_prefilterShader->setInt("environmentMap", 0);
            m_prefilterShader->setMat4("projection", captureProjection);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemap);

            glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
            unsigned int maxMipLevels = 5;
            for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
                // reisze framebuffer according to mip-level size.
                unsigned int mipWidth = static_cast<unsigned int>(
                    128 * std::pow(0.5, mip));
                unsigned int mipHeight = static_cast<unsigned int>(
                    128 * std::pow(0.5, mip));
                glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);
                glRenderbufferStorage(
                    GL_RENDERBUFFER,
                    GL_DEPTH_COMPONENT24,
                    mipWidth,
                    mipHeight);
                glViewport(0, 0, mipWidth, mipHeight);

                float roughness = (float) mip / (float) (maxMipLevels - 1);
                m_prefilterShader->setFloat("roughness", roughness);
                for (unsigned int i = 0; i < 6; ++i) {
                    m_prefilterShader->setMat4("view", captureViews[i]);
                    glFramebufferTexture2D(
                        GL_FRAMEBUFFER,
                        GL_COLOR_ATTACHMENT0,
                        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                        m_prefilterMap,
                        mip);

                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    m_cube.Draw();
                }
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // pbr: generate a 2D LUT from the BRDF equations used.
            // ----------------------------------------------------
            glGenTextures(1, &m_brdfLUTTexture);

            // pre-allocate enough memory for the LUT texture.
            glBindTexture(GL_TEXTURE_2D, m_brdfLUTTexture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RG16F,
                512,
                512,
                0,
                GL_RG,
                GL_FLOAT,
                0);
            // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
            glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);
            glRenderbufferStorage(
                GL_RENDERBUFFER,
                GL_DEPTH_COMPONENT24,
                512,
                512);
            glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D,
                m_brdfLUTTexture,
                0);

            glViewport(0, 0, 512, 512);
            m_brdfShader->use();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderQuad();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // initialize static shader uniforms before rendering
            // --------------------------------------------------
            glm::mat4 projection = sc::Engine::GetInstance()->m_window->
                GetProjectionMatrix(false);
            m_pbrShader->use();
            m_pbrShader->setMat4("projection", projection);
            m_backgroundShader->use();
            m_backgroundShader->setMat4("projection", projection);

            // then before rendering, configure the viewport to the original framebuffer's screen dimensions
            int scrWidth, scrHeight;
            glfwGetFramebufferSize(
                sc::Engine::GetInstance()->m_window->m_window,
                &scrWidth,
                &scrHeight);
            glViewport(0, 0, scrWidth, scrHeight);

        }

        void PreDraw(const glm::mat4 &view) {

            m_pbrShader->use();
            m_pbrShader->setMat4("view", view);
            m_pbrShader->setVec3(
                "camPos",
                sc::Engine::GetInstance()->m_camera->m_position);

            // bind pre-computed IBL data
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMap);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_brdfLUTTexture);

            //Render Stuff

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, m_AlbedoMap);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, m_NormalMap);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, m_MetallicMap);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D, m_RoughnessMap);
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, m_AOMap);

            auto model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0, 0.0, 2.0));
            m_pbrShader->setMat4("model", model);
            m_pbrShader->setMat3(
                "normalMatrix",
                glm::transpose(glm::inverse(glm::mat3(model))));
            renderSphere();

            // render light source (simply re-render sphere at light positions)
            // this looks a bit off as we use the same shader, but it'll make their positions obvious and
            // keeps the codeprint small.
            for (unsigned int i = 0;
                 i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
                glm::vec3 newPos = lightPositions[i] + glm::vec3(
                                       sin(glfwGetTime() * 5.0) * 5.0,
                                       0.0,
                                       0.0);
                // newPos = lightPositions[i];
                m_pbrShader->setVec3(
                    "lightPositions[" + std::to_string(i) + "]",
                    newPos);
                m_pbrShader->setVec3(
                    "lightColors[" + std::to_string(i) + "]",
                    lightColors[i]);

                model = glm::mat4(1.0f);
                model = glm::translate(model, newPos);
                model = glm::scale(model, glm::vec3(0.5f));
                m_pbrShader->setMat4("model", model);
                m_pbrShader->setMat3(
                    "normalMatrix",
                    glm::transpose(glm::inverse(glm::mat3(model))));
                renderSphere();
            }

            // render skybox (render as last to prevent overdraw)
            m_backgroundShader->use();

            m_backgroundShader->setMat4("view", view);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemap);
            //glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
            //glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // display prefilter map
            m_cube.Draw();

            // render BRDF map to screen
            // m_brdfShader->use();
            // m_quad.Draw();

            // // // render BRDF map to screen
            // m_brdfShader->use();
            // m_quad.Draw();
        }


        // utility function for loading a 2D texture from file
        // ---------------------------------------------------
        unsigned int loadTexture(char const *path) {
            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;
            unsigned char *data = stbi_load(
                path,
                &width,
                &height,
                &nrComponents,
                0);
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
                    data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(
                    GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(
                    GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);

                stbi_image_free(data);
            } else {
                std::cout << "Texture failed to load at path: " << path <<
                    std::endl;
                stbi_image_free(data);
            }

            return textureID;
        }

    private:
        // void LoadShaders(const std::string &shaderPath) {
        //     m_pbrShader = new sc::Shader(
        //         (shaderPath + "pbr.vert").c_str(),
        //         (shaderPath + "pbr.frag").c_str());
        //     m_equirectangularToCubemapShader = new sc::Shader(
        //         (shaderPath + "cubemap.vert").c_str(),
        //         (shaderPath + "equirectangular_to_cubemap.frag").c_str());
        //     m_irradianceShader = new sc::Shader(
        //         (shaderPath + "cubemap.vert").c_str(),
        //         (shaderPath + "irradiance_convolution.frag").c_str());
        //     m_prefilterShader = new sc::Shader(
        //         (shaderPath + "cubemap.vert").c_str(),
        //         (shaderPath + "prefilter.frag").c_str());
        //     m_brdfShader = new sc::Shader(
        //         (shaderPath + "brdf.vert").c_str(),
        //         (shaderPath + "brdf.frag").c_str());
        //     m_backgroundShader = new sc::Shader(
        //         (shaderPath + "background.vert").c_str(),
        //         (shaderPath + "background.frag").c_str());
        //
        //     m_pbrShader->use();
        //     m_pbrShader->setInt("irradianceMap", 0);
        //     m_pbrShader->setInt("prefilterMap", 1);
        //     m_pbrShader->setInt("brdfLUT", 2);
        //     m_pbrShader->setInt("albedoMap", 3);
        //     m_pbrShader->setInt("normalMap", 4);
        //     m_pbrShader->setInt("metallicMap", 5);
        //     m_pbrShader->setInt("roughnessMap", 6);
        //     m_pbrShader->setInt("aoMap", 7);
        //
        //     m_backgroundShader->use();
        //     m_backgroundShader->setInt("environmentMap", 0);
        //
        // }
        //
        // void SetupFrameBuffer() {
        //     glGenFramebuffers(1, &m_captureFBO);
        //     glGenRenderbuffers(1, &m_captureRBO);
        //
        //     glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
        //     glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);
        //     glRenderbufferStorage(
        //         GL_RENDERBUFFER,
        //         GL_DEPTH_COMPONENT24,
        //         512,
        //         512);
        //     glFramebufferRenderbuffer(
        //         GL_FRAMEBUFFER,
        //         GL_DEPTH_ATTACHMENT,
        //         GL_RENDERBUFFER,
        //         m_captureRBO);
        // }
        //
        // void LoadHDREnvironmentMap(const std::string &hdrMap) {
        //     stbi_set_flip_vertically_on_load(true);
        //     int width, height, nrComponents;
        //     float *data = stbi_loadf(
        //         hdrMap.c_str(),
        //         &width,
        //         &height,
        //         &nrComponents,
        //         0);
        //     if (data) {
        //         glGenTextures(1, &m_hdrTexture);
        //         glBindTexture(GL_TEXTURE_2D, m_hdrTexture);
        //         glTexImage2D(
        //             GL_TEXTURE_2D,
        //             0,
        //             GL_RGB16F,
        //             width,
        //             height,
        //             0,
        //             GL_RGB,
        //             GL_FLOAT,
        //             data);
        //         // note how we specify the texture's data value to be float
        //
        //         glTexParameteri(
        //             GL_TEXTURE_2D,
        //             GL_TEXTURE_WRAP_S,
        //             GL_CLAMP_TO_EDGE);
        //         glTexParameteri(
        //             GL_TEXTURE_2D,
        //             GL_TEXTURE_WRAP_T,
        //             GL_CLAMP_TO_EDGE);
        //         glTexParameteri(
        //             GL_TEXTURE_2D,
        //             GL_TEXTURE_MIN_FILTER,
        //             GL_LINEAR);
        //         glTexParameteri(
        //             GL_TEXTURE_2D,
        //             GL_TEXTURE_MAG_FILTER,
        //             GL_LINEAR);
        //
        //         stbi_image_free(data);
        //     } else {
        //         std::cout << "Failed to load HDR image." << std::endl;
        //     }
        //
        // }
        //
        // void SetupCubemap() {
        //     glGenTextures(1, &m_envCubemap);
        //     glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemap);
        //     for (unsigned int i = 0; i < 6; ++i) {
        //         glTexImage2D(
        //             GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        //             0,
        //             GL_RGB16F,
        //             512,
        //             512,
        //             0,
        //             GL_RGB,
        //             GL_FLOAT,
        //             nullptr);
        //     }
        //     glTexParameteri(
        //         GL_TEXTURE_CUBE_MAP,
        //         GL_TEXTURE_WRAP_S,
        //         GL_CLAMP_TO_EDGE);
        //     glTexParameteri(
        //         GL_TEXTURE_CUBE_MAP,
        //         GL_TEXTURE_WRAP_T,
        //         GL_CLAMP_TO_EDGE);
        //     glTexParameteri(
        //         GL_TEXTURE_CUBE_MAP,
        //         GL_TEXTURE_WRAP_R,
        //         GL_CLAMP_TO_EDGE);
        //     glTexParameteri(
        //         GL_TEXTURE_CUBE_MAP,
        //         GL_TEXTURE_MIN_FILTER,
        //         GL_LINEAR_MIPMAP_LINEAR);
        //     // enable pre-filter mipmap sampling (combatting visible dots artifact)
        //     glTexParameteri(
        //         GL_TEXTURE_CUBE_MAP,
        //         GL_TEXTURE_MAG_FILTER,
        //         GL_LINEAR);
        // }
        //
        // void SetupCaptureProjectionAndViewMatrices() {
        //     captureProjection = glm::perspective(
        //         glm::radians(90.0f),
        //         1.0f,
        //         0.1f,
        //         10.0f);
        //     captureViews =
        //     {
        //         glm::lookAt(
        //             glm::vec3(0.0f, 0.0f, 0.0f),
        //             glm::vec3(1.0f, 0.0f, 0.0f),
        //             glm::vec3(0.0f, -1.0f, 0.0f)),
        //         glm::lookAt(
        //             glm::vec3(0.0f, 0.0f, 0.0f),
        //             glm::vec3(-1.0f, 0.0f, 0.0f),
        //             glm::vec3(0.0f, -1.0f, 0.0f)),
        //         glm::lookAt(
        //             glm::vec3(0.0f, 0.0f, 0.0f),
        //             glm::vec3(0.0f, 1.0f, 0.0f),
        //             glm::vec3(0.0f, 0.0f, 1.0f)),
        //         glm::lookAt(
        //             glm::vec3(0.0f, 0.0f, 0.0f),
        //             glm::vec3(0.0f, -1.0f, 0.0f),
        //             glm::vec3(0.0f, 0.0f, -1.0f)),
        //         glm::lookAt(
        //             glm::vec3(0.0f, 0.0f, 0.0f),
        //             glm::vec3(0.0f, 0.0f, 1.0f),
        //             glm::vec3(0.0f, -1.0f, 0.0f)),
        //         glm::lookAt(
        //             glm::vec3(0.0f, 0.0f, 0.0f),
        //             glm::vec3(0.0f, 0.0f, -1.0f),
        //             glm::vec3(0.0f, -1.0f, 0.0f))
        //     };
        // }
        //
        // void ConvertEquirectangularToCubemap() {
        //     m_equirectangularToCubemapShader->use();
        //     m_equirectangularToCubemapShader->setInt("equirectangularMap", 0);
        //     m_equirectangularToCubemapShader->setMat4(
        //         "projection",
        //         captureProjection);
        //     glActiveTexture(GL_TEXTURE0);
        //     glBindTexture(GL_TEXTURE_2D, m_hdrTexture);
        //
        //     glViewport(0, 0, 512, 512);
        //     // don't forget to configure the viewport to the capture dimensions.
        //     glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
        //     for (unsigned int i = 0; i < 6; ++i) {
        //         m_equirectangularToCubemapShader->setMat4(
        //             "view",
        //             captureViews[i]);
        //         glFramebufferTexture2D(
        //             GL_FRAMEBUFFER,
        //             GL_COLOR_ATTACHMENT0,
        //             GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        //             m_envCubemap,
        //             0);
        //         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        //         m_cube.Draw();
        //     }
        //     glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //
        //     // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
        //     glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemap);
        //     glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        // }
        //
        // void SetupIrradianceCubemap() {
        //     glGenTextures(1, &m_irradianceMap);
        //     glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);
        //     for (unsigned int i = 0; i < 6; ++i) {
        //         glTexImage2D(
        //             GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        //             0,
        //             GL_RGB16F,
        //             32,
        //             32,
        //             0,
        //             GL_RGB,
        //             GL_FLOAT,
        //             nullptr);
        //     }
        //     glTexParameteri(
        //         GL_TEXTURE_CUBE_MAP,
        //         GL_TEXTURE_WRAP_S,
        //         GL_CLAMP_TO_EDGE);
        //     glTexParameteri(
        //         GL_TEXTURE_CUBE_MAP,
        //         GL_TEXTURE_WRAP_T,
        //         GL_CLAMP_TO_EDGE);
        //     glTexParameteri(
        //         GL_TEXTURE_CUBE_MAP,
        //         GL_TEXTURE_WRAP_R,
        //         GL_CLAMP_TO_EDGE);
        //     glTexParameteri(
        //         GL_TEXTURE_CUBE_MAP,
        //         GL_TEXTURE_MIN_FILTER,
        //         GL_LINEAR);
        //     glTexParameteri(
        //         GL_TEXTURE_CUBE_MAP,
        //         GL_TEXTURE_MAG_FILTER,
        //         GL_LINEAR);
        //
        //     glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
        //     glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);
        //     glRenderbufferStorage(
        //         GL_RENDERBUFFER,
        //         GL_DEPTH_COMPONENT24,
        //         32,
        //         32);
        // }
        //
        // void SolveDiffuseIntegralByConvolution() {
        //     m_irradianceShader->use();
        //     m_irradianceShader->setInt("environmentMap", 0);
        //     m_irradianceShader->setMat4("projection", captureProjection);
        //     glActiveTexture(GL_TEXTURE0);
        //     glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemap);
        //
        //     glViewport(0, 0, 32, 32);
        //     // don't forget to configure the viewport to the capture dimensions.
        //     glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
        //     for (unsigned int i = 0; i < 6; ++i) {
        //         m_irradianceShader->setMat4("view", captureViews[i]);
        //         glFramebufferTexture2D(
        //             GL_FRAMEBUFFER,
        //             GL_COLOR_ATTACHMENT0,
        //             GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        //             m_irradianceMap,
        //             0);
        //         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        //         m_cube.Draw();
        //     }
        //     glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // }
        //
        // void LoadTextures(const std::string &texturePath) {
        //     // std::map<std::string, unsigned int> *textures = new std::map<
        //     //     std::string, unsigned int>();
        //     // textures->insert(
        //     //     std::pair<std::string, int>(
        //     //         "albedo",
        //     //         sc::TextureFromFile("albedo.png", texturePath)));
        //     // textures->insert(
        //     //     std::pair<std::string, int>(
        //     //         "ao",
        //     //         sc::TextureFromFile("ao.png", texturePath)));
        //     // textures->insert(
        //     //     std::pair<std::string, int>(
        //     //         "metallic",
        //     //         sc::TextureFromFile("metallic.png", texturePath)));
        //     // textures->insert(
        //     //     std::pair<std::string, int>(
        //     //         "normal",
        //     //         sc::TextureFromFile("normal.png", texturePath)));
        //     // textures->insert(
        //     //     std::pair<std::string, int>(
        //     //         "roughness",
        //     //         sc::TextureFromFile("roughness.png", texturePath)));
        //
        // }

        // renders (and builds at first invocation) a sphere
        // -------------------------------------------------
        unsigned int sphereVAO = 0;
        GLsizei indexCount;

        void renderSphere() {
            if (sphereVAO == 0) {
                glGenVertexArrays(1, &sphereVAO);

                unsigned int vbo, ebo;
                glGenBuffers(1, &vbo);
                glGenBuffers(1, &ebo);

                std::vector<glm::vec3> positions;
                std::vector<glm::vec2> uv;
                std::vector<glm::vec3> normals;
                std::vector<unsigned int> indices;

                const unsigned int X_SEGMENTS = 64;
                const unsigned int Y_SEGMENTS = 64;
                const float PI = 3.14159265359f;
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
                        float xSegment = (float) x / (float) X_SEGMENTS;
                        float ySegment = (float) y / (float) Y_SEGMENTS;
                        float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(
                                         ySegment * PI);
                        float yPos = std::cos(ySegment * PI);
                        float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(
                                         ySegment * PI);

                        positions.push_back(glm::vec3(xPos, yPos, zPos));
                        uv.push_back(glm::vec2(xSegment, ySegment));
                        normals.push_back(glm::vec3(xPos, yPos, zPos));
                    }
                }

                bool oddRow = false;
                for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
                    if (!oddRow) // even rows: y == 0, y == 2; and so on
                    {
                        for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                            indices.push_back(y * (X_SEGMENTS + 1) + x);
                            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                        }
                    } else {
                        for (int x = X_SEGMENTS; x >= 0; --x) {
                            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                            indices.push_back(y * (X_SEGMENTS + 1) + x);
                        }
                    }
                    oddRow = !oddRow;
                }
                indexCount = static_cast<GLsizei>(indices.size());

                std::vector<float> data;
                for (unsigned int i = 0; i < positions.size(); ++i) {
                    data.push_back(positions[i].x);
                    data.push_back(positions[i].y);
                    data.push_back(positions[i].z);
                    if (normals.size() > 0) {
                        data.push_back(normals[i].x);
                        data.push_back(normals[i].y);
                        data.push_back(normals[i].z);
                    }
                    if (uv.size() > 0) {
                        data.push_back(uv[i].x);
                        data.push_back(uv[i].y);
                    }
                }
                glBindVertexArray(sphereVAO);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(
                    GL_ARRAY_BUFFER,
                    data.size() * sizeof(float),
                    &data[0],
                    GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
                glBufferData(
                    GL_ELEMENT_ARRAY_BUFFER,
                    indices.size() * sizeof(unsigned int),
                    &indices[0],
                    GL_STATIC_DRAW);
                unsigned int stride = (3 + 2 + 3) * sizeof(float);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(
                    0,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    stride,
                    (void *) 0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(
                    1,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    stride,
                    (void *) (3 * sizeof(float)));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(
                    2,
                    2,
                    GL_FLOAT,
                    GL_FALSE,
                    stride,
                    (void *) (6 * sizeof(float)));
            }

            glBindVertexArray(sphereVAO);
            glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
        }

        // renderQuad() renders a 1x1 XY quad in NDC
        // -----------------------------------------
        unsigned int quadVAO = 0;
        unsigned int quadVBO;

        void renderQuad() {
            if (quadVAO == 0) {
                float quadVertices[] = {
                    // positions        // texture Coords
                    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                };
                // setup plane VAO
                glGenVertexArrays(1, &quadVAO);
                glGenBuffers(1, &quadVBO);
                glBindVertexArray(quadVAO);
                glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
                glBufferData(
                    GL_ARRAY_BUFFER,
                    sizeof(quadVertices),
                    &quadVertices,
                    GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(
                    0,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    5 * sizeof(float),
                    (void *) 0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(
                    1,
                    2,
                    GL_FLOAT,
                    GL_FALSE,
                    5 * sizeof(float),
                    (void *) (3 * sizeof(float)));
            }
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
        }
};


#endif //PBR_H
