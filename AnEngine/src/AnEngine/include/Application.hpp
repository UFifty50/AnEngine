#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.hpp"
#include "LayerStack.hpp"
#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"
#include "ImGui/ImGuiLayer.hpp"

namespace AnEngine {
    class AE_API Application {
    private:
        static Application* instance;
        std::unique_ptr<Window> window;
        ImGuiLayer* imGuiLayer;
        bool running = true;
        LayerStack layerStack;

        bool onWindowClose(WindowCloseEvent& e);

    public:
        Application();
        virtual ~Application();

        void onEvent(Event& e);
        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);

        static inline Application& get() { return *instance; }
        inline Window& getWindow() { return *window; }

        void Run();
    };

    // user defined
    Application* CreateApplication();
}

#endif
