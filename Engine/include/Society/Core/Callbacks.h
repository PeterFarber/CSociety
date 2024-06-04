//
// Created by Peter Farber on 5/16/24.
//

#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <GLFW/glfw3.h>

namespace sc {
    void window_close_callback(GLFWwindow *window);

    void window_size_callback(
        GLFWwindow *window,
        const int width,
        const int height
    );

    void framebuffer_size_callback(
        GLFWwindow *window,
        const int width,
        const int height
    );

    void window_content_scale_callback(
        GLFWwindow *window,
        const float xscale,
        const float yscale
    );

    void window_pos_callback(
        GLFWwindow *window,
        const int xpos,
        const int ypos
    );

    void window_iconify_callback(
        GLFWwindow *window,
        const int iconified
    );

    void window_maximize_callback(
        GLFWwindow *window,
        const int maximized
    );

    void window_focus_callback(GLFWwindow *window, const int focused);

    void window_refresh_callback(GLFWwindow *window);

    void key_callback(
        GLFWwindow *window,
        const int key,
        const int scancode,
        int action,
        int mods
    );

    void character_callback(
        GLFWwindow *window,
        unsigned int codepoint
    );

    void cursor_position_callback(
        GLFWwindow *window,
        double xpos,
        double ypos
    );

    void cursor_enter_callback(GLFWwindow *window, const int entered);

    void mouse_button_callback(
        GLFWwindow *window,
        int button,
        int action,
        int mods
    );

    void scroll_callback(
        GLFWwindow *window,
        double xoffset,
        double yoffset
    );

    void joystick_callback(int jid, const int event);

    void drop_callback(
        GLFWwindow *window,
        int count,
        const char **paths
    );
}

#endif //CALLBACKS_H
