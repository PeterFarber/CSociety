//
// Created by Peter Farber on 5/13/24.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <Society/Components/Component.h>

namespace sc {
    class InputSystem;
    class TransformComponent;

    class ControllerComponent final : public Component {
        public:
            TransformComponent *m_transform;
            InputSystem *m_input;

            explicit ControllerComponent(Entity *entity);

            void Update(double deltaTime) override;
    };
}

#endif
