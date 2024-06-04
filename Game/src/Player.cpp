// //
// // Created by Peter Farber on 5/13/24.
// //
//
// #include "Player.h"
//
// #include <Society/Components/SpriteComponent.h>
// #include <Society/Components/TransformComponent.h>
// #include <Society/Components/ControllerComponent.h>
// #include <Society/Components/BoundingBoxDebugComponent.h>
// #include <GLFW/glfw3.h>
//
// Player::Player() {
//     m_components.push_back(new sc::TransformComponent(this));
//
//     m_components.push_back(new sc::ControllerComponent(this));
//     m_components.push_back(
//         new sc::BoundingBoxDebugComponent(
//             this,
//             "bounding"
//         )
//     );
//     m_components.push_back(
//         new sc::SpriteComponent(
//             this,
//             "sprite",
//             "face"
//         )
//     );
// }
//
// void Player::Update(double deltaTime) {
//     // auto *transform = FindComponent<sc::TransformComponent>();
//     const auto *sprite = FindComponent<sc::SpriteComponent>();
//     sprite->m_shader->SetFloat("time", static_cast<float>(glfwGetTime()));
//     // transform->m_rotation.y += static_cast<float>(deltaTime * 25.0f);
//     // glm::mat4 model(1.0f);
//     // model = glm::translate(model, transform->m_position);
//     // model = glm::translate(
//     //     model, glm::vec3(0.5f * transform->m_scale.x,
//     //                      0.5f * transform->m_scale.y, 0.0f));
//     // model = glm::rotate(
//     //     model, glm::radians(static_cast<float>(deltaTime * 25.0f)),
//     //     glm::vec3(0.0f, 1.0f, 0.0f));
//     // model = glm::translate(
//     //     model, glm::vec3(-0.5f * transform->m_scale.x,
//     //                      -0.5f * transform->m_scale.y, 0.0f));
//     //
//     Entity::Update(deltaTime);
// }
//
