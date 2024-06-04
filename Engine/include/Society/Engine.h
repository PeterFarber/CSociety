//
// Created by Peter Farber on 5/16/24.
//

#ifndef SC_ENGINE_H
#define SC_ENGINE_H
#include <Society/Interfaces/ISingleton.h>
#include <Society/Core/Base.h>

namespace sc {
    class Camera;
    class Window;
    // class ResourceManager;
    class InputSystem;

    class DECLSPEC Engine : public ISingleton<Engine> {
        public:
            InputSystem *m_input;
            // ResourceManager *m_resourceManager;
            Window *m_window;
            Camera *m_camera;

            Engine();

            ~Engine();

            bool Init();
    };
}

#endif //ENGINE_H
