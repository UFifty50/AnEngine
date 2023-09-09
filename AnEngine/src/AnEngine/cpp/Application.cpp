#include "aepch.hpp"

#include "Application.hpp"

#include "Core/Input.hpp"
#include "Core/Log.hpp"
#include "Core/Random.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Renderer/RenderAPI.hpp"
#include "Renderer/Renderer.hpp"
#include "Time/Time.hpp"
#include "Time/TimeStep.hpp"
#include "Window.hpp"


namespace AnEngine {
    Application* Application::Application::instance = nullptr;

    Application::Application(const std::string& name) {
        AE_PROFILE_FUNCTION()

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
            AE_CORE_ASSERT(false, msg.str().c_str());
        }

        window = Scope<Window>(Window::create(WindowProperties(name)));
        window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

        Renderer::init();
        Random::init();

        imGuiLayer = MakeRef<ImGuiLayer>();
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
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
        dispatcher.dispatch<WindowResizeEvent>(
            BIND_EVENT_FN(Application::onWindowResize));

        for (auto it = layerStack.end(); it != layerStack.begin();) {
            (*--it)->onEvent(e);
            if (e.handled) break;
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& closeEvent) {
        running = false;
        return true;
    }

    bool Application::onWindowResize(WindowResizeEvent& resizeEvent) {
        if (resizeEvent.getWidth() == 0 || resizeEvent.getHeight() == 0) {
            minimized = true;
            return false;
        }

        Renderer::onWindowResize(resizeEvent.getWidth(), resizeEvent.getHeight());

        return false;
    }

    int Application::Run() {
        while (running) {
            float time = Time::getTime();
            TimeStep deltaTime = time - lastFrameTime;
            lastFrameTime = time;

            if (!minimized) {
                for (Ref<Layer> layer : layerStack) {
                    layer->onUpdate(deltaTime);
                }
            }

            imGuiLayer->begin();
            for (Ref<Layer> layer : layerStack) {
                layer->onImGuiRender();
            }
            imGuiLayer->end();

            window->onUpdate();
        }

        return this->exitCode;
    }

    void Application::Shutdown(int exitCode) {
        AE_PROFILE_FUNCTION()

        running = false;
        this->exitCode = exitCode;
    }

    int main(int argc, char** argv) {
        Log::init();

        AE_PROFILE_BEGIN_SESSION("Startup", "AnEngineProfile-Startup.json");
        auto app = CreateApplication();
        AE_PROFILE_END_SESSION()

        AE_PROFILE_BEGIN_SESSION("Runtime", "AnEngineProfile-Runtime.json");
        int exitCode = app->Run();
        AE_PROFILE_END_SESSION()

        AE_PROFILE_BEGIN_SESSION("Shutdown", "AnEngineProfile-Shutdown.json");
        delete app;
        AE_PROFILE_END_SESSION()

        AE_PROFILE_END_SESSION()

        return exitCode;
    }
}  // namespace AnEngine
