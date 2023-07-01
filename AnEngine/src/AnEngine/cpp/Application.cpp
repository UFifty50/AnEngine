#include "aepch.hpp"
#include "Application.hpp"
#include "Renderer/Renderer.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Renderer/Buffers/IndexBuffer.hpp"
#include "Renderer/Buffers/BufferLayout.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"
#include "Input.hpp"
#include "Log.hpp"

#include <glad/glad.h>
#include <glm/mat4x4.hpp>


namespace AnEngine {
    Application* Application::Application::instance = nullptr;

    Application::Application() {
        AE_CORE_ASSERT(!instance, "Application already exists!");
        instance = this;

        if (Renderer::getAPI() == RenderAPI::NoAPI) {
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


      //  vertexArray.reset(VertexArray::create());
       
      //  float vertices[3 * 7] = {
     //       -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
    //         0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
    //         0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
     //   };

      //  vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));

     //   BufferLayout layout = {
     //       { ShaderDataType::Vec3, "position" },
     //       { ShaderDataType::Float4, "colour" }
     //   };

       // vertexBuffer->setLayout(layout);
      //  vertexArray->addVertexBuffer(vertexBuffer);

      //  uint32_t indices[3] = { 0, 1, 2 };
   //     indexBuffer.reset(IndexBuffer::create(indices, 3));
    //    vertexArray->setIndexBuffer(indexBuffer);


        squareVA.reset(VertexArray::create());

        BufferLayout layout = {
            { ShaderDataType::Vec3, "position" },
        };

        float vertices[3 * 4] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
        };

        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::create(vertices, sizeof(vertices)));

        squareVB->setLayout(layout);
        squareVA->addVertexBuffer(squareVB);

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<IndexBuffer> squareIB;
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
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

         //   shader->bind();
            shader->bind();
            squareVA->bind();
            glDrawElements(GL_TRIANGLES, squareVA->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
        //    vertexArray->bind();

        //    glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

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
