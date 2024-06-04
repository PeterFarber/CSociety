// //
// // Created by Peter Farber on 5/13/24.
// //
//
// #include <glad/glad.h>
//
// #include "RayMarch.h"
//
// #include <Society/Components/SpriteComponent.h>
// #include <Society/Components/TransformComponent.h>
// #include <Society/Components/ControllerComponent.h>
// #include <Society/Components/BoundingBoxDebugComponent.h>
// #include <glm/ext/matrix_transform.hpp>
//
// #include "../../Engine/include/Society/Camera.h"
// #include "Society/Core/Window.h"
// #include "Society/Managers/ResourceManager.h"
//
// RayMarch::RayMarch(): m_transform(), m_shader() {
//     m_transform = new sc::TransformComponent(this);
//     m_components.push_back(m_transform);
//     m_shader = sc::ResourceManager::GetShader("raymarch");
//     m_texture = sc::ResourceManager::GetTexture("noise");
//     VAO = -1;
//
//     // sc::ResourceManager::GetShader("raymarch").Use().SetInteger(
//     //     "iChannel",
//     //     static_cast<int>(m_texture.ID)
//     // );
//
//     float vertices[] = {
//         -1.0f,
//         -1.0f,
//         -1.0f,
//         0.0f,
//         0.0f,
//         1.0f,
//         -1.0f,
//         -1.0f,
//         1.0f,
//         0.0f,
//         1.0f,
//         1.0f,
//         -1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         -1.0f,
//         1.0f,
//         1.0f,
//         -1.0f,
//         1.0f,
//         -1.0f,
//         0.0f,
//         1.0f,
//         -1.0f,
//         -1.0f,
//         -1.0f,
//         0.0f,
//         0.0f,
//         -1.0f,
//         -1.0f,
//         1.0f,
//         0.0f,
//         0.0f,
//         1.0f,
//         -1.0f,
//         1.0f,
//         1.0f,
//         0.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         -1.0f,
//         1.0f,
//         1.0f,
//         0.0f,
//         1.0f,
//         -1.0f,
//         -1.0f,
//         1.0f,
//         0.0f,
//         0.0f,
//         -1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         0.0f,
//         -1.0f,
//         1.0f,
//         -1.0f,
//         1.0f,
//         1.0f,
//         -1.0f,
//         -1.0f,
//         -1.0f,
//         0.0f,
//         1.0f,
//         -1.0f,
//         -1.0f,
//         -1.0f,
//         0.0f,
//         1.0f,
//         -1.0f,
//         -1.0f,
//         1.0f,
//         0.0f,
//         0.0f,
//         -1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         0.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         0.0f,
//         1.0f,
//         1.0f,
//         -1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         -1.0f,
//         -1.0f,
//         0.0f,
//         1.0f,
//         1.0f,
//         -1.0f,
//         -1.0f,
//         0.0f,
//         1.0f,
//         1.0f,
//         -1.0f,
//         1.0f,
//         0.0f,
//         0.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         0.0f,
//         -1.0f,
//         -1.0f,
//         -1.0f,
//         0.0f,
//         1.0f,
//         1.0f,
//         -1.0f,
//         -1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         -1.0f,
//         1.0f,
//         1.0f,
//         0.0f,
//         1.0f,
//         -1.0f,
//         1.0f,
//         1.0f,
//         0.0f,
//         -1.0f,
//         -1.0f,
//         1.0f,
//         0.0f,
//         0.0f,
//         -1.0f,
//         -1.0f,
//         -1.0f,
//         0.0f,
//         1.0f,
//         -1.0f,
//         1.0f,
//         -1.0f,
//         0.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         -1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         0.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         1.0f,
//         0.0f,
//         -1.0f,
//         1.0f,
//         1.0f,
//         0.0f,
//         0.0f,
//         -1.0f,
//         1.0f,
//         -1.0f,
//         0.0f,
//         1.0f
//     };
//
//     unsigned int VBO;
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//
//     glBindVertexArray(VAO);
//
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//     // position attribute
//     glVertexAttribPointer(
//         0,
//         3,
//         GL_FLOAT,
//         GL_FALSE,
//         5 * sizeof(float),
//         (void *) 0
//     );
//     glEnableVertexAttribArray(0);
//     // texture coord attribute
//     glVertexAttribPointer(
//         1,
//         2,
//         GL_FLOAT,
//         GL_FALSE,
//         5 * sizeof(float),
//         (void *) (3 * sizeof(float))
//     );
//     glEnableVertexAttribArray(1);
// }
//
// void RayMarch::Update(double deltaTime) {
//     m_shader.Use();
//     glm::mat4 model(1.0f);
//     model = glm::translate(model, m_transform->m_position);
//     // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
//
//     // model = glm::translate(
//     //     model, glm::vec3(0.5f * m_transform->m_scale.x,
//     //                      0.5f * m_transform->m_scale.y, 0.0f));
//     model = glm::rotate(
//         model,
//         glm::radians(m_transform->m_rotation.x),
//         glm::vec3(1.0f, 0.0f, 0.0f)
//     ); // then rotate
//     model = glm::rotate(
//         model,
//         glm::radians(m_transform->m_rotation.y),
//         glm::vec3(0.0f, 1.0f, 0.0f)
//     );
//     model = glm::rotate(
//         model,
//         glm::radians(m_transform->m_rotation.z),
//         glm::vec3(0.0f, 0.0f, 1.0f)
//     );
//     // model = glm::translate(
//     //     model, glm::vec3(-0.5f * m_transform->m_scale.x,
//     //                      -0.5f * m_transform->m_scale.y, 0.0f));
//
//     model = glm::scale(model, m_transform->m_scale); // last scale
//     glm::mat4 proj = sc::Window::GetInstance()->GetProjectionMatrix(false);
//     m_shader.SetMatrix4("projection", proj);
//
//     glm::mat4 view = Camera::GetInstance()->GetViewMatrix();
//     m_shader.SetMatrix4("view", view);
//     m_shader.SetMatrix4("model", model);
//
//     m_shader.SetVector2f("u_resolution", 1280, 768);
//     m_shader.SetFloat("u_time", static_cast<float>(glfwGetTime()));
//     glActiveTexture(GL_TEXTURE0);
//     m_texture.Bind();
//
//     glBindVertexArray(this->VAO);
//     glDrawArrays(GL_TRIANGLES, 0, 36);
//     glBindVertexArray(0);
//     Entity::Update(deltaTime);
// }
//
