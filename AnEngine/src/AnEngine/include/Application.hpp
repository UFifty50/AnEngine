#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Events/ApplicationEvent.hpp"
#include "Events/Event.hpp"
#include "File/InputFileStream.hpp"
#include "ImGui/ImGuiLayer.hpp"
#include "LayerStack.hpp"
#include "Window.hpp"


namespace AnEngine {
    class AE_API Application {
    public:
        Application();
        ~Application();

        void onEvent(Event& e);
        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);

        static inline Application& get() { return *instance; }
        inline Window& getWindow() { return *window; }

        void Run();

    private:
        static Application* instance;
        Scope<Window> window;
        ImGuiLayer* imGuiLayer;
        bool running = true;
        LayerStack layerStack;
        float lastFrameTime = 0;

        bool onWindowClose(WindowCloseEvent& e);
    };

    // user defined
    Application* CreateApplication();
}  // namespace AnEngine

#endif
