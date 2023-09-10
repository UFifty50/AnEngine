#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Core/LayerStack.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/Event.hpp"
#include "File/InputFileStream.hpp"
#include "ImGui/ImGuiLayer.hpp"
#include "Window.hpp"


namespace AnEngine {

    class Application {
    public:
        struct Data {
            bool initialized = false;

            Scope<Window> window;
            Ref<ImGuiLayer> imGuiLayer;
            bool minimized = false;
            bool running = true;
            LayerStack layerStack;
            float lastFrameTime = 0;

            int exitCode = 0;
        };

        static Data applicationData;

        static void Init(const std::string& name = "AnEngine");
        static int Run();
        static void Shutdown(int exitCode = 0);

        static void onEvent(Event& e);
        static void pushLayer(Ref<Layer> layer);
        static void pushOverlay(Ref<Layer> overlay);

        static Window& getWindow() { return *applicationData.window; }
        static Ref<ImGuiLayer> getImGuiLayer() { return applicationData.imGuiLayer; }


    private:
        static bool onWindowClose(WindowCloseEvent& closeEvent);
        static bool onWindowResize(WindowResizeEvent& resizeEvent);
    };

    int main(int argc, char** argv);

    // user defined
    Application* CreateApplication();
}  // namespace AnEngine

#endif
