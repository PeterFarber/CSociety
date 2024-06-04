#include "Game.h"
#include <Society.h>
#include <Society/Engine.h>
#include <Society/Model.h>
// sc::Model cube;
#include <iostream>
#include <fstream>
#include <sstream>

#include <Society/Meshes/Sphere.h>

#include "PBR.h"
#include "Society/Meshes/Cube.h"
#include "Society/Meshes/Quad.h"

void renderSphere();
void renderCube();
void renderQuad();
unsigned int loadTexture(char const *path);


Game::Game(/* args */) {
    m_engine = sc::Engine::GetInstance();
    sc::Log::Init();
    SC_CORE_WARN("Initialized Log!");
}

Game::~Game() {
    delete m_engine;
    for (const auto entity: m_entities) {
        delete entity;
    }
}

void Game::Init(unsigned int width, unsigned int height, const char *title) {
    // // load shaders
    // m_engine->m_resourceManager->LoadShader(
    // 	"resources/shaders/raymarch/brdf.vert",
    // 	"resources/shaders/raymarch/equirectangular_to_cubemap.frag",
    // 	nullptr,
    // 	"raymarch"
    // );
    // cube.loadObj("resources/models/armadillo.obj");
    // cube.Generate();

    // m_engine->m_resourceManager->LoadShader(
    //     "resources/shaders/brdf.vert",
    //     "resources/shaders/equirectangular_to_cubemap.frag",
    //     nullptr,
    //     "sprite"
    //     );
    //
    // m_engine->m_resourceManager->LoadShader(
    //     "resources/shaders/bounding/brdf.vert",
    //     "resources/shaders/bounding/equirectangular_to_cubemap.frag",
    //     nullptr,
    //     "bounding"
    //     );
    //
    // m_engine->m_resourceManager->LoadShader(
    // 	"/Users/peterfarber/CLionProjects/Society/Game/resources/shaders/basic/brdf.vert",
    // 	"/Users/peterfarber/CLionProjects/Society/Game/resources/shaders/basic/equirectangular_to_cubemap.frag",
    // 	nullptr,
    // 	"basic"
    // );

    // m_engine->m_resourceManager->LoadShader(
    //     "/Users/peterfarber/CLionProjects/Society/Game/resources/shaders/model/brdf.vert",
    //     "/Users/peterfarber/CLionProjects/Society/Game/resources/shaders/model/equirectangular_to_cubemap.frag",
    //     nullptr,
    //     "model"
    //     );

    // configure shaders
    // sc::ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    // m_engine->m_resourceManager->GetShader("sprite")->Use().SetInteger(
    //     "image",
    //     0
    //     );

    // m_engine->m_resourceManager->GetShader("basic")->Use().SetInteger(
    //     "materialTex",
    //     0
    //     );

    // sc::ResourceManager::GetShader("sprite").SetMatrix4(
    // 	"projection", m_window->GetProjectionMatrix(true));
    // set render-specific controls
    // load textures
    // sc::ResourceManager::LoadTexture(
    // 	"resources/textures/noise.png",
    // 	false,
    // 	"noise"
    // );
    // m_engine->m_resourceManager->LoadTexture(
    //     "resources/textures/Sprite-0002.png",
    //     true,
    //     "face"
    //     );
    // m_engine->m_resourceManager->LoadTexture(
    //     "resources/textures/Skull.jpg",
    //     false,
    //     "face2"
    //     );

    // m_entities.push_back(new Player());
}

void Game::Update(const double dt) const {
    sc::Camera *c = m_engine->m_camera;
    const sc::InputSystem *i = m_engine->m_input;
    c->m_movementSpeed = 20;
    if (i->IsKeyPressed(GLFW_KEY_UP) || i->IsKeyHeld(GLFW_KEY_UP)) {
        c->ProcessKeyboard(
            sc::Camera_Movement::FORWARD,
            static_cast<float>(dt)
            );
    } else if (i->IsKeyPressed(GLFW_KEY_DOWN) || i->IsKeyHeld(GLFW_KEY_DOWN)) {
        c->ProcessKeyboard(
            sc::Camera_Movement::BACKWARD,
            static_cast<float>(dt)
            );
    }

    if (i->IsKeyPressed(GLFW_KEY_LEFT) || i->IsKeyHeld(GLFW_KEY_LEFT)) {
        c->ProcessKeyboard(
            sc::Camera_Movement::LEFT,
            static_cast<float>(dt)
            );
    } else if (i->IsKeyPressed(GLFW_KEY_RIGHT) || i->
               IsKeyHeld(GLFW_KEY_RIGHT)) {
        c->ProcessKeyboard(
            sc::Camera_Movement::RIGHT,
            static_cast<float>(dt)
            );
    }
    // sc::Shader *s = sc::Engine::GetInstance()->m_resourceManager->GetShader(
    // 	"basic"
    // );
    // s->Use();
    // glActiveTexture(GL_TEXTURE0);
    // m_engine->m_resourceManager->GetTexture("face2")->Bind();
    // // glBindTexture(GL_TEXTURE_2D, 0);
    // const glm::mat4 proj = sc::Engine::GetInstance()->m_window->
    // 	GetProjectionMatrix(false);
    // s->
    // 	SetMatrix4(
    // 		"projection",
    // 		proj
    // 	);
    //
    // const glm::mat4 view = sc::Engine::GetInstance()->m_camera->GetViewMatrix();
    // s->SetMatrix4("view", view);
    // s->SetMatrix4("model", glm::mat4(1.0f));
    //
    // s->SetFloat("materialShininess", 80.0f);
    // s->SetVector3f("materialSpecularColor", glm::vec3(1.0f, 1.0f, 1.0f));
    // s->SetVector3f("light.position", glm::vec3(0.0f, -10.0f, 0.0f));
    // s->SetVector3f("light.intensities", glm::vec3(1, 1, 1));
    // s->SetFloat("light.attenuation", 0.2f);
    // s->SetFloat("light.ambientCoefficient", 0.005f);;
    // s->SetVector3f(
    // 	"cameraPosition",
    // 	sc::Engine::GetInstance()->m_camera->m_position
    // );
    // s->SetInteger("blinn", i->IsKeyHeld(GLFW_KEY_B));
    //
    // cube.Draw();

    // for (const auto entity: m_entities) {
    //     entity->Update(dt);
    // }

    // sc::Shader *s = sc::Engine::GetInstance()->m_resourceManager->GetShader(
    //     "model"
    //     );

}

void Game::Run() {

    sc::Window *window = m_engine->m_window;
    PBR pbr("textures/cobblestone_street_night_8k.hdr");
    //
    // std::map<std::string, unsigned int> *textures = new std::map<
    //     std::string, unsigned int>();
    // textures->insert(
    //     std::pair<std::string, int>(
    //         "albedo",
    //         sc::TextureFromFile(
    //             "albedo.png",
    //             RESOURCE_LOCATION + "textures/brick_wall")));
    // textures->insert(
    //     std::pair<std::string, int>(
    //         "ao",
    //         sc::TextureFromFile(
    //             "ao.png",
    //             RESOURCE_LOCATION + "textures/brick_wall")));
    // textures->insert(
    //     std::pair<std::string, int>(
    //         "metallic",
    //         sc::TextureFromFile(
    //             "metallic.png",
    //             RESOURCE_LOCATION + "textures/brick_wall")));
    // textures->insert(
    //     std::pair<std::string, int>(
    //         "normal",
    //         sc::TextureFromFile(
    //             "normal.png",
    //             RESOURCE_LOCATION + "textures/brick_wall")));
    // textures->insert(
    //     std::pair<std::string, int>(
    //         "roughness",
    //         sc::TextureFromFile(
    //             "roughness.png",
    //             RESOURCE_LOCATION + "textures/brick_wall")));

    // std::string dir = "/Users/peterfarber/CLionProjects/Society/Game/resources";

    // sc::Model ourModel(
    //     dir + "/models/BrickBox/firstbox.obj");
    // sc::Shader pbrShader(
    //     (dir + "/shaders/pbr/brdf.vert").c_str(),
    //     (dir + "/shaders/pbr/equirectangular_to_cubemap.frag").c_str()
    //     );
    //
    // pbrShader.use();
    // pbrShader.setInt("irradianceMap", 0);
    // pbrShader.setInt("prefilterMap", 1);
    // pbrShader.setInt("brdfLUT", 2);
    // pbrShader.setInt("albedoMap", 3);
    // pbrShader.setInt("normalMap", 4);
    // pbrShader.setInt("metallicMap", 5);
    // pbrShader.setInt("roughnessMap", 6);
    // pbrShader.setInt("aoMap", 7);
    // // lights
    // // ------
    // glm::vec3 lightPositions[] = {
    //     glm::vec3(0.0f, 5.0f, 10.0f),
    //     // glm::vec3(-5.0f, -5.0f, 10.0f),
    //     // glm::vec3(5.0f, 5.0f, 10.0f),
    //     // glm::vec3(10.0f, 10.0f, 10.0f),
    //     // glm::vec3(-10.0f, -10.0f, 10.0f),
    //     // glm::vec3(10.0f, -10.0f, 10.0f)
    // };
    // glm::vec3 lightColors[] = {
    //     glm::vec3(100.0f, 100.0f, 100.0f),
    //     // glm::vec3(0.0f, 0.0f, 200.0f),
    //     // glm::vec3(200.0f, 0.0f, 0.0f),
    //     // glm::vec3(100.0f, 100.0f, 100.0f),
    //     // glm::vec3(100.0f, 100.0f, 100.0f)
    // };
    // int nrRows = 7;
    // int nrColumns = 7;
    // float spacing = 2.5;
    //
    // const glm::mat4 proj =
    //     sc::Engine::GetInstance()->m_window->GetProjectionMatrix(false);
    // pbrShader.use();
    // pbrShader.setMat4("projection", proj);

    // sc::Sphere sphere;
    // sc::Quad quad;
    // sc::Cube cube;
    // lights
    // ------

    while (!window->ShouldClose()) {
        const double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        window->Clear();

        const glm::mat4 view =
            sc::Engine::GetInstance()->m_camera->GetViewMatrix();

        pbr.PreDraw(view);

        // auto model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(3.0, 0.0, 2.0));
        // pbr.m_pbrShader->setMat4("model", model);
        // pbr.m_pbrShader->setMat3(
        //     "normalMatrix",
        //     glm::transpose(glm::inverse(glm::mat3(model))));
        // sphere.Draw();

        // render light source (simply re-render sphere at light positions)
        // this looks a bit off as we use the same shader, but it'll make their positions obvious and
        // keeps the codeprint small.
        // for (unsigned int i = 0;
        //      i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
        //     glm::vec3 newPos = lightPositions[i] + glm::vec3(
        //                            sin(glfwGetTime() * 5.0) * 5.0,
        //                            0.0,
        //                            0.0);
        //     newPos = lightPositions[i];
        //     pbr.m_pbrShader->setVec3(
        //         "lightPositions[" + std::to_string(i) + "]",
        //         newPos);
        //     pbr.m_pbrShader->setVec3(
        //         "lightColors[" + std::to_string(i) + "]",
        //         lightColors[i]);
        //
        //     auto model = glm::mat4(1.0f);
        //     model = glm::translate(model, newPos);
        //     model = glm::scale(model, glm::vec3(0.5f));
        //     pbr.m_pbrShader->setMat4("model", model);
        //     pbr.m_pbrShader->setMat3(
        //         "normalMatrix",
        //         glm::transpose(glm::inverse(glm::mat3(model))));
        //     sphere.Draw();
        // }

        // pbr.PostDraw(view);
        // for (int row = 0; row < nrRows; ++row) {
        //     for (int col = 0; col < nrColumns; ++col) {
        //         // we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
        //         // on direct lighting.
        //
        //         model = glm::mat4(1.0f);
        //         model = glm::translate(
        //             model,
        //             glm::vec3(
        //                 (float) (col - (nrColumns / 2)) * spacing,
        //                 (float) (row - (nrRows / 2)) * spacing,
        //                 -3.0f
        //                 ));
        //         pbrShader.setMat4("model", model);
        //         pbrShader.setMat3(
        //             "normalMatrix",
        //             glm::transpose(glm::inverse(glm::mat3(model))));
        //         renderSphere();
        //     }
        // }
        // // model = glm::mat4(1.0f);
        // // model = glm::translate(model, glm::vec3(-1.0, 0.0, 2.0));
        // // pbrShader.setMat4("model", model);
        // // pbrShader.setMat3(
        // //     "normalMatrix",
        // //     glm::transpose(glm::inverse(glm::mat3(model))));
        // // renderSphere();
        // // render light source (simply re-render sphere at light positions)
        // // this looks a bit off as we use the same shader, but it'll make their positions obvious and
        // // keeps the codeprint small.
        // for (unsigned int i = 0;
        //      i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
        //     glm::vec3 newPos = lightPositions[i] + glm::vec3(
        //                            sin(glfwGetTime() * 1.0) * 15.0,
        //                            0.0,
        //                            0.0);
        //     // newPos = lightPositions[i];
        //     pbrShader.setVec3(
        //         "lightPositions[" + std::to_string(i) + "]",
        //         newPos);
        //     pbrShader.setVec3(
        //         "lightColors[" + std::to_string(i) + "]",
        //         lightColors[i]);
        //
        //     model = glm::mat4(1.0f);
        //     model = glm::translate(model, newPos);
        //     model = glm::scale(model, glm::vec3(0.5f));
        //     pbrShader.setMat4("model", model);
        //     pbrShader.setMat3(
        //         "normalMatrix",
        //         glm::transpose(glm::inverse(glm::mat3(model))));
        //     sphere.Draw();
        // }
        Update(deltaTime);

        window->SwapBuffers();
    }

}
