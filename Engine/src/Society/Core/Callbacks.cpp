//
// Created by Peter Farber on 5/16/24.
//
#include <Society/Camera.h>
#include <Society/Core/Callbacks.h>
#include <Society/Core/Log.h>
#include <Society/Managers/InputSystem.h>
#include <GLFW/glfw3.h>
#include <Society/Engine.h>

namespace sc {
    void window_close_callback(GLFWwindow *window) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    void window_size_callback(
        GLFWwindow *window,
        const int width,
        const int height
    ) {
        glViewport(0, 0, width, height);
    }

    void framebuffer_size_callback(
        GLFWwindow *window,
        const int width,
        const int height
    ) {
        glViewport(0, 0, width, height);
    }

    void window_content_scale_callback(
        GLFWwindow *window,
        const float xscale,
        const float yscale
    ) {}

    void window_pos_callback(
        GLFWwindow *window,
        const int xpos,
        const int ypos
    ) {}

    void window_iconify_callback(
        GLFWwindow *window,
        const int iconified
    ) {}

    void window_maximize_callback(
        GLFWwindow *window,
        const int maximized
    ) {}

    void window_focus_callback(GLFWwindow *window, const int focused) {}

    void window_refresh_callback(GLFWwindow *window) {
        glfwSwapBuffers(window);
    }

    void key_callback(
        GLFWwindow *window,
        const int key,
        const int scancode,
        int action,
        int mods
    ) {
        Engine::GetInstance()->m_input->SetKeyState(
            key,
            (action)
        );
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    void character_callback(
        GLFWwindow *window,
        unsigned int codepoint
    ) {}

    void cursor_position_callback(
        GLFWwindow *window,
        double xpos,
        double ypos
    ) {
        glm::dvec2 offset = Engine::GetInstance()->m_input->GetMouseOffset(
            xpos,
            ypos
        );
        Engine::GetInstance()->m_camera->ProcessMouseMovement(
            offset.x,
            offset.y
        );
    }

    void cursor_enter_callback(GLFWwindow *window, const int entered) {}

    void mouse_button_callback(
        GLFWwindow *window,
        const int button,
        const int action,
        int mods
    ) {
        Engine::GetInstance()->m_input->SetMouseState(
            button,
            (action)
        );
    }

    void scroll_callback(
        GLFWwindow *window,
        double xoffset,
        double yoffset
    ) {}

    void joystick_callback(int jid, const int event) {}

    void drop_callback(
        GLFWwindow *window,
        int count,
        const char **paths
    ) {
        SC_CORE_INFO("Drop: {0}", count);
        for (int i = 0; i < count; i++) {
            SC_CORE_INFO("Path: {0}", paths[i]);
        }
    }
}
