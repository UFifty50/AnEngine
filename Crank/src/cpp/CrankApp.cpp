#include "Application.hpp"
#include "CrankEditor.hpp"
#include "EntryPoint.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine::Crank {
    class CrankApp : public Application {
    public:
        CrankApp() { pushLayer(std::make_shared<CrankEditor>()); }

        ~CrankApp() {}
    };
}  // namespace AnEngine::Crank


namespace AnEngine {
    Application* CreateApplication() {
        RenderAPI::setAPI(RenderAPI::OpenGL);
        Application::Init("Crank - AnEngine");
        return new Crank::CrankApp();
    }
}  // namespace AnEngine
