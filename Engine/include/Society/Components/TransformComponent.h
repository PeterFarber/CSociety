//
// Created by Peter Farber on 5/13/24.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/vec3.hpp>
#include <Society/Components/Component.h>
#include <Society/Core/Base.h>

namespace sc {
    class DECLSPEC TransformComponent final : public Component {
        public:
            glm::vec3 m_position;
            glm::vec3 m_rotation;
            glm::vec3 m_scale;

            explicit TransformComponent(Entity *entity);

            void Update(double deltaTime) override;
    };
}

#endif //TRANSFORM_H
