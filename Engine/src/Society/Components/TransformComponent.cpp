//
// Created by Peter Farber on 5/13/24.
//
#include <Society/Components/TransformComponent.h>

#include "Society/Components/Component.h"

namespace sc {
    TransformComponent::TransformComponent(Entity *entity): m_position(0.0f),
        m_rotation(0.0f),
        m_scale(1.0f) {
        m_entity = entity;
    }

    void TransformComponent::Update(double deltaTime) {
        Component::Update(deltaTime);
    }
}
