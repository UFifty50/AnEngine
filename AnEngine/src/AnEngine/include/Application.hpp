#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "aepch.hpp"
#include "Events/Event.hpp"
#include "Window.hpp"

namespace AnEngine {
    class AE_API Application {
    private:
        std::unique_ptr<Window> window;
        bool running = true;

        bool onWindowClose(WindowCloseEvent& e);

    public:
        Application();
        virtual ~Application();

        void onEvent(Event& e);
        void Run();
    };

    // user defined
    Application* CreateApplication();
}

#endif
