#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Core/LayerStack.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/Event.hpp"
#include "File/InputFileStream.hpp"
#include "ImGui/ImGuiLayer.hpp"
#include "Window.hpp"


namespace AnEngine {
    class AE_API Application {
    public:
        Application(const std::string& name = "AnEngine");
        ~Application();

        void onEvent(Event& e);
        void pushLayer(Ref<Layer> layer);
        void pushOverlay(Ref<Layer> overlay);

        static inline Application& get() { return *instance; }
        inline Window& getWindow() { return *window; }

        int Run();
        void Shutdown(int exitCode = 0);

    private:
        static Application* instance;
        Scope<Window> window;
        Ref<ImGuiLayer> imGuiLayer;
        bool minimized = false;
        bool running = true;
        LayerStack layerStack;
        float lastFrameTime = 0;

        int exitCode = 0;

        bool onWindowClose(WindowCloseEvent& closeEvent);
        bool onWindowResize(WindowResizeEvent& resizeEvent);
    };

    int main(int argc, char** argv);

    // user defined
    Application* CreateApplication();
}  // namespace AnEngine

#endif
