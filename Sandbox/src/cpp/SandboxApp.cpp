#include <AnEngine.hpp>


class ExampleLayer : public AnEngine::Layer {
public:
    ExampleLayer() : Layer("Example") {}

    void onUpdate() override {
        AE_INFO("ExampleLayer::Update");
    }

    void onEvent(AnEngine::Event& event) override {
        AE_TRACE("{0}", event);
    }
};

class Sandbox : public AnEngine::Application {
public:
    Sandbox() {
        pushLayer(new ExampleLayer());
        pushOverlay(new AnEngine::ImGuiLayer());
    }

	~Sandbox() {}

	void Run() {
		while (true);
	}
};

AnEngine::Application* AnEngine::CreateApplication() {
    return new Sandbox();
}
