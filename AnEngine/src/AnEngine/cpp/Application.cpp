#include "aepch.hpp"
#include "Application.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Input.hpp"
#include "Log.hpp"

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

namespace AnEngine {
    Application* Application::Application::instance = nullptr;

    Application::Application() {
        AE_CORE_ASSERT(!instance, "Application already exists!");
        instance = this;

        window = std::unique_ptr<Window>(Window::create());
        window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

        imGuiLayer = new ImGuiLayer();
        pushOverlay(imGuiLayer);

        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        float vertices[3 * 3] = {
            -0.7f, -0.7f, 0.0f,
             0.7f, -0.7f, 0.0f,
             0.0f,  0.7f, 0.0f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(float),
            nullptr
        );

        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

        unsigned int indices[3] = { 0, 1, 2 };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        InputFileStream vertShaderStream("res/shaders/basic.vert");
        InputFileStream fragShaderStream("res/shaders/basic.frag");
        shader = std::make_unique<Shader>(vertShaderStream, fragShaderStream);
    }

    Application::~Application() {}

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
            if (e.handled) {
                break;
            }
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
            
            shader->bind();

            glBindVertexArray(vertexArray);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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
