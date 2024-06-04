#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Society/Camera.h>

namespace sc {
    Camera::Camera(): m_position(), m_front(), m_up(), m_right(), m_worldUp(),
                      m_pitch(),
                      m_movementSpeed(),
                      m_mouseSensitivity(),
                      m_zoom() {
        m_position = glm::vec3(0.0f, 0.0f, 10.0f);
        m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_yaw = YAW;
        m_pitch = PITCH;
        m_front = glm::vec3(0.0f, 0.0f, -1.0f);
        m_movementSpeed = SPEED;
        m_mouseSensitivity = SENSITIVITY;
        m_zoom = ZOOM;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 Camera::GetViewMatrix() const {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void Camera::ProcessKeyboard(
        const Camera_Movement direction,
        const float deltaTime
    ) {
        const float velocity = m_movementSpeed * deltaTime;
        if (direction == FORWARD) m_position += m_front * velocity;
        if (direction == BACKWARD) m_position -= m_front * velocity;
        if (direction == LEFT) m_position -= m_right * velocity;
        if (direction == RIGHT) m_position += m_right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void Camera::ProcessMouseMovement(
        float xoffset,
        float yoffset,
        const GLboolean constrainPitch
    ) {
        xoffset *= m_mouseSensitivity;
        yoffset *= m_mouseSensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (m_pitch > 89.0f) m_pitch = 89.0f;
            if (m_pitch < -89.0f) m_pitch = -89.0f;
        }

        // update m_front, m_right and m_up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void Camera::ProcessMouseScroll(float yoffset) {
        m_zoom -= (float) yoffset;
        if (m_zoom < 1.0f) m_zoom = 1.0f;
        if (m_zoom > 45.0f) m_zoom = 45.0f;
    }

    // calculates the front vector from the Camera's (updated) Euler Angles
    void Camera::updateCameraVectors() {
        // calculate the new m_front vector
        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(front);
        // also re-calculate the m_right and m_up vector
        m_right = glm::normalize(glm::cross(m_front, m_worldUp));
        // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }
}
