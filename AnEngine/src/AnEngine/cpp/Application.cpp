#include "aepch.hpp"
#include "Application.hpp"
#include "Renderer/RenderAPI.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/RenderCommandQueue.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Renderer/Buffers/IndexBuffer.hpp"
#include "Renderer/Buffers/BufferLayout.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"
#include "Input.hpp"
#include "Log.hpp"


namespace AnEngine {
    Application* Application::Application::instance = nullptr;

    Application::Application() {
        AE_CORE_ASSERT(!instance, "Application already exists!");
        instance = this;

        if (RenderAPI::getAPI() == RenderAPI::NoAPI) {
            std::stringstream msg;
            msg << "AnEngine::Renderer::setAPI() needs to be called in CreateApplication with one of" << std::endl;
            msg << "| RenderAPI::OpenGL" << std::endl;
            msg << "| RenderAPI::DirectX11" << std::endl;
            msg << "| RenderAPI::DirectX12" << std::endl;
            msg << "| RenderAPI::Vulkan" << std::endl;
            AE_CORE_ASSERT(false, msg.str());
        }

        window = std::unique_ptr<Window>(Window::create());
        window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

        imGuiLayer = new ImGuiLayer();
        pushOverlay(imGuiLayer);


        squareVA.reset(VertexArray::create());

        float vertices[3 * 4] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
        };
        squareVB.reset(VertexBuffer::create(vertices, sizeof(vertices)));

        BufferLayout layout = {
            { ShaderDataType::Vec3, "position" },
        };
        squareVB->setLayout(layout);

        squareVA->addVertexBuffer(squareVB);

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        squareIB.reset(IndexBuffer::create(indices, 6));

        squareVA->setIndexBuffer(squareIB);


        InputFileStream vertShaderStream("res/shaders/basic.vert");
        InputFileStream fragShaderStream("res/shaders/basic.frag");
        shader.reset(Shader::create(vertShaderStream, fragShaderStream));
    }

    Application::~Application() = default;

    void Application::pushLayer(Layer* layer) {
        layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer* overlay) {
        layerStack.pushOverlay(overlay);
        overlay->onAttach();
    }

    void Application::onEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

        for (auto it = layerStack.end(); it != layerStack.begin();) {
            (*--it)->onEvent(e);
            if (e.handled) break;
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& e) {
        running = false;
        return true;
    }

    void Application::Run() {
        while (running) {
            RenderCommandQueue::clearColour({ 0.1f, 0.1f, 0.1f, 1 });
            RenderCommandQueue::clear();

            Renderer::beginScene();

            shader->bind();
            Renderer::submit(squareVA);

            Renderer::endScene();


            for (Layer* layer : layerStack) {
                layer->onUpdate();
            }

            imGuiLayer->begin();
            for (Layer* layer : layerStack) {
                layer->onImGuiRender();
            }
            imGuiLayer->end();

            window->onUpdate();
        }
     }
}
