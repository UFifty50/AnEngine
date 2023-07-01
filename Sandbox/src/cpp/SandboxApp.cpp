#include <AnEngine.hpp>
#include "imgui.h"
#include "imgui_internal.h"


class ExampleLayer : public AnEngine::Layer {
public:
    ExampleLayer() : Layer("Example") {}

    void onUpdate() override {
        AE_INFO("ExampleLayer::Update");

        if (AnEngine::Input::isKeyPressed(AE_KEY_ENTER)) {
            AE_ERROR("ENTER is pressed (poll)!");
        }
    }

    virtual void onImGuiRender() override {
        ImGui::Begin("Test");
        ImGui::Text("Hello World!");
        ImGui::End();
    }

    void onEvent(AnEngine::Event& event) override {
     //   AE_TRACE("{0}", event);
    }
};

class Sandbox : public AnEngine::Application {
public:
    Sandbox() {
        pushLayer(new ExampleLayer());
    }

    ~Sandbox() {}
};

AnEngine::Application* AnEngine::CreateApplication() {
    Renderer::setAPI(RenderAPI::OpenGL);
    return new Sandbox();
}
