#include <AnEngine.hpp>
#include <AnEngine/include/EntryPoint.hpp>

#include "SandBox2D.hpp"


class Sandbox : public AnEngine::Application {
public:
    Sandbox() { pushLayer(std::make_shared<SandBox2D>()); }

    ~Sandbox() {}
};

AnEngine::Application* AnEngine::CreateApplication() {
    RenderAPI::setAPI(RenderAPI::OpenGL);
    return new Sandbox();
}
