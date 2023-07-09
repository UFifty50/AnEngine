#include "aepch.hpp"

#include "Application.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "Renderer/RenderAPI.hpp"
#include "Time/Time.hpp"
#include "Time/TimeStep.hpp"
#include "Window.hpp"


namespace AnEngine {
    Application* Application::Application::instance = nullptr;

    Application::Application() {
        AE_CORE_ASSERT(!instance, "Application already exists!");
        instance = this;

        if (RenderAPI::getAPI() == RenderAPI::NoAPI) {
            std::stringstream msg;
            msg << "AnEngine::Renderer::setAPI() needs to be called in "
                   "CreateApplication with one "
                   "of"
                << std::endl;
            msg << "| RenderAPI::OpenGL" << std::endl;
            msg << "| RenderAPI::DirectX11" << std::endl;
            msg << "| RenderAPI::DirectX12" << std::endl;
            msg << "| RenderAPI::Vulkan" << std::endl;
            AE_CORE_ASSERT(false, msg.str());
        }

        window = Window::create();
        window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

        imGuiLayer = std::make_shared<ImGuiLayer>();
        pushOverlay(imGuiLayer);
    }

    Application::~Application() = default;

    void Application::pushLayer(Ref<Layer> layer) {
        layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Ref<Layer> overlay) {
        layerStack.pushOverlay(overlay);
        overlay->onAttach();
    }

    void Application::onEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(
            BIND_EVENT_FN(Application::onWindowClose));

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
            float time = Time::getTime();
            TimeStep timeStep = time - lastFrameTime;
            lastFrameTime = time;

            for (Ref<Layer> layer : layerStack) {
                layer->onUpdate(timeStep);
            }

            imGuiLayer->begin();
            for (Ref<Layer> layer : layerStack) {
                layer->onImGuiRender();
            }
            imGuiLayer->end();

            window->onUpdate();
        }
    }
}  // namespace AnEngine
