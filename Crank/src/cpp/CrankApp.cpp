#include "Application.hpp"
#include "CrankEditor.hpp"
#include "EntryPoint.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    class CrankApp : public Application {
    public:
        CrankApp() { pushLayer(std::make_shared<CrankEditor>()); }

        ~CrankApp() {}
    };

    Application* CreateApplication() {
        RenderAPI::setAPI(RenderAPI::OpenGL);
        Application::Init("Crank - AnEngine");
        return new CrankApp();
    }
}  // namespace AnEngine
