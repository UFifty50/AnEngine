#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.hpp"
#include "LayerStack.hpp"
#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"
#include "ImGui/ImGuiLayer.hpp"

#include "File/InputFileStream.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Renderer/Buffers/IndexBuffer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"


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
        std::unique_ptr<Window> window;
        ImGuiLayer* imGuiLayer;
        bool running = true;
        LayerStack layerStack;

        std::shared_ptr<Shader> shader;
        std::shared_ptr<VertexArray> vertexArray;
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<IndexBuffer> indexBuffer;

        std::shared_ptr<VertexArray> squareVA;

        bool onWindowClose(WindowCloseEvent& e);
    };

    // user defined
    Application* CreateApplication();
}

#endif
