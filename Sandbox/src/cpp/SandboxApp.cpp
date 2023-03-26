#include <AnEngine.hpp>


class ExampleLayer : public AnEngine::Layer {
public:
    ExampleLayer() : Layer("Example") {}

    void onUpdate() override {
        AE_INFO("ExampleLayer::Update");

        if (AnEngine::Input::isKeyPressed(AE_KEY_ENTER)) {
            AE_ERROR("ENTER is pressed (poll)!");
        }
    }

    void onEvent(AnEngine::Event& event) override {
     //   AE_TRACE("{0}", event);
    }
};

class Sandbox : public AnEngine::Application {
public:
    Sandbox() {
        pushLayer(new ExampleLayer());
        pushOverlay(new AnEngine::ImGuiLayer());
    }

    ~Sandbox() {}
};

AnEngine::Application* AnEngine::CreateApplication() {
    return new Sandbox();
}
