//
// Created by Peter Farber on 5/16/24.
//
#include <Society/Camera.h>
#include <Society/Engine.h>
#include <Society/Core/Window.h>
#include <Society/Managers/InputSystem.h>
// #include <Society/Managers/ResourceManager.h>

namespace sc {
    Engine::Engine() {
        m_window = new Window(1280, 768, "Engine");
        m_input = new InputSystem();
        // m_resourceManager = new ResourceManager();
        m_camera = new Camera();
    }

    Engine::~Engine() {
        delete m_input;
        // delete m_resourceManager;
        delete m_window;
        delete m_camera;
    }

    bool Engine::Init() {
    }
}
