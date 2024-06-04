//
// Created by Peter Farber on 5/13/24.
//

#ifndef SPRITE_H
#define SPRITE_H
#include <string>
#include <glm/vec3.hpp>
#include <Society/Shader.h>
#include <Society/Texture.h>
#include <Society/Components/Component.h>

namespace sc {
    class TransformComponent;

    class SpriteComponent final : public Component {
        public:
            TransformComponent *m_transform;

            glm::vec3 m_color;
            Texture2D *m_texture;
            Shader *m_shader;
            unsigned int quadVAO;

            SpriteComponent(
                Entity *entity,
                const std::string &shader,
                const std::string &texture
            );

            void Update(double deltaTime) override;
    };
}

#endif //SPRITE_H
