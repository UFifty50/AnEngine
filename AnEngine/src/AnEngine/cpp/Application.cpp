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
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

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
