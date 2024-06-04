// //
// // Created by Peter Farber on 5/13/24.
// //
// #include <glad/glad.h>
// #include <glm/ext/matrix_transform.hpp>
// #include <Society/Camera.h>
// #include <Society/Engine.h>
// #include <Society/Entity.h>
// #include <Society/Components/BoundingBoxDebugComponent.h>
// #include <Society/Components/TransformComponent.h>
// #include <Society/Core/Window.h>
// #include <Society/Managers/ResourceManager.h>
//
// namespace sc {
//     BoundingBoxDebugComponent::BoundingBoxDebugComponent(
//         Entity *entity,
//         const std::string &shader
//     ): m_shader() {
//         m_entity = entity;
//         m_transform = m_entity->FindComponent<TransformComponent>();
//         m_shader = Engine::GetInstance()->m_resourceManager->GetShader(shader);
//         quadVAO = -1;
//
//         constexpr float vertices[] = {
//             -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
//             1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
//             1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
//             1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
//             -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
//             -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
//
//             -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
//             1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
//             1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//             1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//             -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//             -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
//
//             -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//             -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
//             -1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
//             -1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
//             -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
//             -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//
//             1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//             1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
//             1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
//             1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
//             1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
//             1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//
//             -1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
//             1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
//             1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
//             1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
//             -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
//             -1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
//
//             -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
//             1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
//             1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//             1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//             -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
//             -1.0f, 1.0f, -1.0f, 0.0f, 1.0f
//         };
//
//         unsigned int VBO;
//         glGenVertexArrays(1, &quadVAO);
//         glGenBuffers(1, &VBO);
//
//         glBindVertexArray(quadVAO);
//
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferData(
//             GL_ARRAY_BUFFER,
//             sizeof(vertices),
//             vertices,
//             GL_STATIC_DRAW
//         );
//
//         // position attribute
//         glVertexAttribPointer(
//             0,
//             3,
//             GL_FLOAT,
//             GL_FALSE,
//             5 * sizeof(float),
//             static_cast<void *>(nullptr)
//         );
//         glEnableVertexAttribArray(0);
//         // texture coord attribute
//         glVertexAttribPointer(
//             1,
//             2,
//             GL_FLOAT,
//             GL_FALSE,
//             5 * sizeof(float),
//             reinterpret_cast<void *>(3 * sizeof(float))
//         );
//         glEnableVertexAttribArray(1);
//     }
//
//     void BoundingBoxDebugComponent::Update(double deltaTime) {
//         // prepare transformations
//         m_shader->Use();
//         glm::mat4 model(1.0f);
//         model = glm::translate(model, m_transform->m_position);
//         // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
//
//         // model = glm::translate(
//         //     model, glm::vec3(0.5f * m_transform->m_scale.x,
//         //                      0.5f * m_transform->m_scale.y, 0.0f));
//         model = glm::rotate(
//             model,
//             glm::radians(m_transform->m_rotation.x),
//             glm::vec3(1.0f, 0.0f, 0.0f)
//         ); // then rotate
//         model = glm::rotate(
//             model,
//             glm::radians(m_transform->m_rotation.y),
//             glm::vec3(0.0f, 1.0f, 0.0f)
//         );
//         model = glm::rotate(
//             model,
//             glm::radians(m_transform->m_rotation.z),
//             glm::vec3(0.0f, 0.0f, 1.0f)
//         );
//         // model = glm::translate(
//         //     model, glm::vec3(-0.5f * m_transform->m_scale.x,
//         //                      -0.5f * m_transform->m_scale.y, 0.0f));
//
//         model = glm::scale(model, m_transform->m_scale); // last scale
//         const glm::mat4 proj = Engine::GetInstance()->m_window->
//             GetProjectionMatrix(false);
//         m_shader->SetMatrix4("projection", proj);
//
//         const glm::mat4 view = Engine::GetInstance()->m_camera->GetViewMatrix();
//         m_shader->SetMatrix4("view", view);
//         m_shader->SetMatrix4("model", model);
//
//         glBindVertexArray(this->quadVAO);
//         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//         glDrawArrays(GL_TRIANGLES, 0, 36);
//         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//         glBindVertexArray(0);
//     }
// }
