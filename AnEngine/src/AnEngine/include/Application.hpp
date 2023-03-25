#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.hpp"
#include "LayerStack.hpp"
#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"

namespace AnEngine {
    class AE_API Application {
    private:
        std::unique_ptr<Window> window;
        bool running = true;
        LayerStack layerStack;

        bool onWindowClose(WindowCloseEvent& e);

    public:
        Application();
        virtual ~Application();

        void onEvent(Event& e);
        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);

        void Run();
    };

    // user defined
    Application* CreateApplication();
}

#endif
