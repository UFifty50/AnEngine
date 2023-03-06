#include "aepch.hpp"
#include "Application.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Log.hpp"


namespace AnEngine {
    Application::Application() {}
    Application::~Application() {}

    void Application::Run() {
        AnEngine::WindowResizeEvent e(1280, 720);
        AE_CRITICAL(e);
        while (true);
     }
}
