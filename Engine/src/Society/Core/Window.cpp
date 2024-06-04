#include <glad/glad.h>

#include <Society/Core/Window.h>
#include <Society/Core/Log.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <Society/Camera.h>
#include <Society/Core/Events.h>
#include <Society/Core/Callbacks.h>

namespace sc {
    void Window::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::SwapBuffers() const {
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glm::mat4 Window::GetProjectionMatrix(const bool ortho = false) const {
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        if (ortho) {
            return glm::ortho(
                0.0f,
                static_cast<float>(width),
                static_cast<float>(height),
                0.0f,
                -0.1f,
                100.0f
                );
        }

        return glm::perspective(
            glm::radians(35.0f),
            static_cast<float>(width * 1.0 / height),
            0.1f,
            100.0f
            );
    }

    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(m_window);
    }

    Window::Window() {
    }

    Window::Window(
        const unsigned int width,
        const unsigned int height,
        const char *title
        ) {
        glfwSetErrorCallback(error_callback);

        if (!glfwInit())
            exit(EXIT_FAILURE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
        m_window = glfwCreateWindow(
            static_cast<int>(width),
            static_cast<int>(height),
            title,
            nullptr,
            nullptr
            );
        if (!m_window) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        SetOptions();

        SetCallbacks();

        glfwMakeContextCurrent(m_window);
        if (!gladLoadGLLoader(
            reinterpret_cast<GLADloadproc>(glfwGetProcAddress)
            )) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        // glfwSwapInterval(1);
        SetFunctionality();
    }

    void Window::SetCallbacks() const {
        glfwSetWindowCloseCallback(m_window, window_close_callback);
        glfwSetWindowSizeCallback(m_window, window_size_callback);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        glfwSetCursorPosCallback(m_window, cursor_position_callback);
        glfwSetKeyCallback(m_window, key_callback);
        glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    }

    void Window::SetOptions() const {
        // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
        if (glfwRawMouseMotionSupported()) {
            SC_CORE_INFO("Raw mouse motion supported");
            glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
        // tell GLFW to capture our mouse
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Window::SetFunctionality() {
        // configure global opengl state
        // -----------------------------
        glEnable(GL_DEPTH_TEST);
        // set depth function to less than AND equal for skybox depth trick.
        glDepthFunc(GL_LEQUAL);
        // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    }

    void Window::error_callback(int error, const char *description) {
        SC_CORE_ERROR("Error: {0}", description);
    }

    Window::~Window() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
}
