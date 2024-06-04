#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Society/Interfaces/ISingleton.h>

namespace sc {
    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum Camera_Movement {
        FORWARD, BACKWARD, LEFT,
        RIGHT
    };

    // Default camera values
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;

    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class Camera {
        public:
            // camera Attributes
            glm::vec3 m_position;
            glm::vec3 m_front;
            glm::vec3 m_up;
            glm::vec3 m_right;
            glm::vec3 m_worldUp;
            // euler Angles
            float m_yaw;
            float m_pitch;
            // camera options
            float m_movementSpeed;
            float m_mouseSensitivity;
            float m_zoom;

            // constructor with vectors
            Camera();

            // returns the view matrix calculated using Euler Angles and the LookAt Matrix
            glm::mat4 GetViewMatrix() const;

            // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
            void ProcessKeyboard(Camera_Movement direction, float deltaTime);

            // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
            void ProcessMouseMovement(
                float xoffset,
                float yoffset,
                GLboolean constrainPitch = true
            );

            // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
            void ProcessMouseScroll(float yoffset);

        private:
            // calculates the front vector from the Camera's (updated) Euler Angles
            void updateCameraVectors();
    };
}
#endif
