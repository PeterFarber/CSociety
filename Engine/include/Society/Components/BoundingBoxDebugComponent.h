//
// Created by Peter Farber on 5/13/24.
//

#ifndef BOUNDINGBOXDEBUG_H
#define BOUNDINGBOXDEBUG_H
#include <Society/Shader.h>
#include <Society/Components/Component.h>

namespace sc {
    class Shader;
    class TransformComponent;

    class BoundingBoxDebugComponent final : public Component {
        public:
            TransformComponent *m_transform;

            Shader *m_shader;
            unsigned int quadVAO;

            BoundingBoxDebugComponent(
                Entity *entity,
                const std::string &shader
            );

            void Update(double deltaTime) override;
    };
}

#endif
