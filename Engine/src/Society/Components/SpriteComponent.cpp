// //
// // Created by Peter Farber on 5/13/24.
// //
// #include <Society/Components/SpriteComponent.h>
// #include <glad/glad.h>
//
// #include <glm/ext/matrix_transform.hpp>
// #include <Society/Camera.h>
// #include <Society/Engine.h>
// #include <Society/Components/Component.h>
// #include <Society/Components/TransformComponent.h>
// #include <Society/Managers/ResourceManager.h>
// #include <Society/Entity.h>
// #include <Society/Core/Window.h>
//
// namespace sc {
//     SpriteComponent::SpriteComponent(
//         Entity *entity,
//         const std::string &shader,
//         const std::string &texture
//     ): m_color(),
//        m_shader() {
//         m_entity = entity;
//
//         m_transform = m_entity->FindComponent<
//             TransformComponent>();
//         m_color = {1, 1, 1};
//         m_shader = Engine::GetInstance()->m_resourceManager->GetShader(shader);
//         m_texture = Engine::GetInstance()->m_resourceManager->GetTexture(
//             texture
//         );
//         quadVAO = -1;
//
//         glm::vec3 size(
//             m_texture->Width / 6,
//             m_texture->Height,
//             0.0f
//         );
//         size = glm::normalize(size);
//         m_transform->m_scale =
//             glm::vec3(size.x, size.y, size.x * 0.5f);
//
//         unsigned int VBO;
//
//         constexpr float vertices[] = {
//             1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
//             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//             -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//
//             1.0f, -1.0f, 0.0f, 1.0f, -0.0f,
//             -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
//             -1.0f, 1.0f, 0.0f, 0.0f, 1.0f
//         };
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
//     void SpriteComponent::Update(double deltaTime) {
//         // prepare transformations
//         m_shader->Use();
//         glm::mat4 model(1.0f);
//         model = glm::translate(model, m_transform->m_position);
//
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
//
//         model = glm::scale(model, m_transform->m_scale); // last scale
//         const glm::mat4 proj = Engine::GetInstance()->m_window->
//             GetProjectionMatrix(false);
//         m_shader->
//             SetMatrix4(
//                 "projection",
//                 proj
//             );
//
//         const glm::mat4 view = Engine::GetInstance()->m_camera->GetViewMatrix();
//         m_shader->SetMatrix4("view", view);
//         m_shader->SetMatrix4("model", model);
//         m_shader->SetFloat("time", glfwGetTime());
//
//         // render textured quad
//         m_shader->SetVector3f("spriteColor", m_color);
//
//         glActiveTexture(GL_TEXTURE0);
//         m_texture->Bind();
//
//         glBindVertexArray(this->quadVAO);
//         glDrawArrays(GL_TRIANGLES, 0, 6);
//         glBindVertexArray(0);
//     }
// }
