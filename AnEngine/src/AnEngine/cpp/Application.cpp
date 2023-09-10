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
    Application::Data Application::applicationData;

    void Application::Init(const std::string& name) {
        AE_PROFILE_FUNCTION()

        AE_CORE_ASSERT(!applicationData.initialized, "Application already exists!");
        applicationData.initialized = true;

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

        applicationData.window = Scope<Window>(Window::create(WindowProperties(name)));
        applicationData.window->setEventCallback(&Application::onEvent);

        Renderer::init();
        Random::init();

        applicationData.imGuiLayer = MakeRef<ImGuiLayer>();
        pushOverlay(applicationData.imGuiLayer);
    }

    // Application::~Application() = default;

    void Application::pushLayer(Ref<Layer> layer) {
        applicationData.layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Ref<Layer> overlay) {
        applicationData.layerStack.pushOverlay(overlay);
        overlay->onAttach();
    }

    void Application::onEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(Application::onWindowClose);
        dispatcher.dispatch<WindowResizeEvent>(Application::onWindowResize);

        for (auto it = applicationData.layerStack.end();
             it != applicationData.layerStack.begin();) {
            (*--it)->onEvent(e);
            if (e.handled) break;
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& closeEvent) {
        applicationData.running = false;
        return true;
    }

    bool Application::onWindowResize(WindowResizeEvent& resizeEvent) {
        if (resizeEvent.getWidth() == 0 || resizeEvent.getHeight() == 0) {
            applicationData.minimized = true;
            return false;
        }

        Renderer::onWindowResize(resizeEvent.getWidth(), resizeEvent.getHeight());

        return false;
    }

    int Application::Run() {
        if (!applicationData.initialized) {
            std::stringstream msg;
            msg << "AnEngine::Application::Init() needs to be called after "
                   "Renderer::setAPI() in CreateApplication"
                << std::endl;
            AE_CORE_ASSERT(false, msg.str().c_str());
            return -200;
        }

        while (applicationData.running) {
            float time = Time::getTime();
            TimeStep deltaTime = time - applicationData.lastFrameTime;
            applicationData.lastFrameTime = time;

            if (!applicationData.minimized) {
                for (Ref<Layer> layer : applicationData.layerStack) {
                    layer->onUpdate(deltaTime);
                }
            }

            applicationData.imGuiLayer->begin();
            for (Ref<Layer> layer : applicationData.layerStack) {
                layer->onImGuiRender();
            }
            applicationData.imGuiLayer->end();

            applicationData.window->onUpdate();
        }

        return applicationData.exitCode;
    }

    void Application::Shutdown(int exitCode) {
        AE_PROFILE_FUNCTION()

        applicationData.running = false;
        applicationData.exitCode = exitCode;
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
