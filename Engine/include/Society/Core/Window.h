#ifndef SC_WINDOW_H
#define SC_WINDOW_H
#define GLFW_INCLUDE_NONE

#include <Society/Core/Base.h>
#include <glm/mat4x4.hpp>
#include <GLFW/glfw3.h>

namespace sc {
    class DECLSPEC Window {
        public:
            /* data */
            GLFWwindow *m_window;

        public:
            Window();

            Window(unsigned int width, unsigned int height, const char *title);

            ~Window();

            void Clear();

            void SwapBuffers() const;

            glm::mat4 GetProjectionMatrix(bool ortho) const;

            bool ShouldClose() const;

            glm::vec2 GetWindowSize() const {
                int width, height;
                glfwGetWindowSize(m_window, &width, &height);
                return glm::vec2(width, height);
            }

        private:
            void SetCallbacks() const;

            void SetOptions() const;

            static void SetFunctionality();

            static void error_callback(int error, const char *description);
    };
} // namespace sc

#endif
