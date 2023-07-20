#include <AnEngine.hpp>
#include <AnEngine/include/EntryPoint.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "imgui.h"
#include "imgui_internal.h"

#include "SandBox2D.hpp"


class Sandbox2D : public AnEngine::Application {
public:
    Sandbox2D() { pushLayer(std::make_shared<SandBox2D>()); }

    ~Sandbox2D() {}
};

AnEngine::Application* AnEngine::CreateApplication() {
    RenderAPI::setAPI(RenderAPI::OpenGL);
    return new Sandbox2D();
}
