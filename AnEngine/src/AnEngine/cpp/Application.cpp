#include "aepch.hpp"
#include "Application.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Log.hpp"

#include <GLFW/glfw3.h>


namespace AnEngine {
    Application::Application() {
        window = std::unique_ptr<Window>(Window::create());
        window->setEventCallback(BIND_EVENT_FN(onEvent));
    }

    Application::~Application() {}

    void Application::onEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
        AE_CORE_DEBUG(e);
    }

    bool Application::onWindowClose(WindowCloseEvent& e) {
        running = false;
        return true;
    }

    void Application::Run() {
        while (running) {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            window->onUpdate();
        }
     }
}
