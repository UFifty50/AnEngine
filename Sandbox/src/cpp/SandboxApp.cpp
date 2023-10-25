#include "Application.hpp"
#include "EntryPoint.hpp"
#include "Renderer/RenderAPI.hpp"
#include "SandBox2D.hpp"


namespace AnEngine {
    class SandboxApp : public Application {
    public:
        SandboxApp() { pushLayer(std::make_shared<SandBox2D>()); }

        ~SandboxApp() {}
    };

    Application* CreateApplication() {
        RenderAPI::setAPI(RenderAPI::OpenGL);
        Application::Init("SandBox - AnEngine");
        return new SandboxApp();
    }
}  // namespace AnEngine
