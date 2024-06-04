//
// Created by Peter Farber on 5/13/24.
//
#include <Society/Components/ControllerComponent.h>

#include <Society/Managers/InputSystem.h>

#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>
#include <Society/Engine.h>
#include <Society/Entity.h>
#include <Society/Components/TransformComponent.h>

namespace sc {
   ControllerComponent::ControllerComponent(Entity *entity) {
      m_entity = entity;

      m_input = Engine::GetInstance()->m_input;
      m_transform = m_entity->FindComponent<
         TransformComponent>();
   }

   void ControllerComponent::Update(double deltaTime) {
      const double speed = 3;

      if (m_input->IsKeyPressed(GLFW_KEY_W) || m_input->IsKeyHeld(GLFW_KEY_W)) {
         m_transform->m_position += glm::vec3(0, (speed * deltaTime), 0);
      } else if (m_input->IsKeyPressed(GLFW_KEY_S) || m_input->
                 IsKeyHeld(GLFW_KEY_S)) {
         m_transform->m_position += glm::vec3(0, -(speed * deltaTime), 0);
      }

      if (m_input->IsKeyPressed(GLFW_KEY_A) || m_input->IsKeyHeld(GLFW_KEY_A)) {
         m_transform->m_position += glm::vec3(-(speed * deltaTime), 0, 0);
      } else if (m_input->IsKeyPressed(GLFW_KEY_D) || m_input->
                 IsKeyHeld(GLFW_KEY_D)) {
         m_transform->m_position += glm::vec3((speed * deltaTime), 0, 0);
      }

      // if (m_input->IsKeyPressed(GLFW_KEY_UP) || m_input->
      //     IsKeyHeld(GLFW_KEY_UP)) {
      //    m_transform->m_position += glm::vec3(0, 0, (speed * deltaTime));
      // } else if (m_input->IsKeyPressed(GLFW_KEY_DOWN) || m_input->
      //            IsKeyHeld(GLFW_KEY_DOWN)) {
      //    m_transform->m_position += glm::vec3(0, 0, -(speed * deltaTime));
      // }
   }
}
