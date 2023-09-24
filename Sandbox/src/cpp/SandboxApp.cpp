#include "Application.hpp"
#include "EntryPoint.hpp"
#include "Renderer/RenderAPI.hpp"
#include "SandBox2D.hpp"


namespace AnEngine {
    class CrankApp : public Application {
    public:
        CrankApp() { pushLayer(std::make_shared<SandBox2D>()); }

        ~CrankApp() {}
    };

    Application* CreateApplication() {
        RenderAPI::setAPI(RenderAPI::OpenGL);
        Application::Init("SandBox - AnEngine");
        return new CrankApp();
    }
}  // namespace AnEngine
