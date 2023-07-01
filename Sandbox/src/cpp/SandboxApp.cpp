#include <AnEngine.hpp>
#include "imgui.h"
#include "imgui_internal.h"


class ExampleLayer : public AnEngine::Layer {
public:
    ExampleLayer() : Layer("Example") {
        squareVA.reset(AnEngine::VertexArray::create());

        float vertices[3 * 4] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
        };
        squareVB.reset(AnEngine::VertexBuffer::create(vertices, sizeof(vertices)));

        AnEngine::BufferLayout layout = {
            { AnEngine::ShaderDataType::Vec3, "position" },
        };
        squareVB->setLayout(layout);

        squareVA->addVertexBuffer(squareVB);

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        squareIB.reset(AnEngine::IndexBuffer::create(indices, 6));

        squareVA->setIndexBuffer(squareIB);


        AnEngine::InputFileStream vertShaderStream("res/shaders/basic.vert");
        AnEngine::InputFileStream fragShaderStream("res/shaders/basic.frag");
        shader.reset(AnEngine::Shader::create(vertShaderStream, fragShaderStream));
    }

    void onUpdate() override {
        AE_INFO("ExampleLayer::Update");

        AnEngine::RenderCommandQueue::clearColour({ 0.1f, 0.1f, 0.1f, 1 });
        AnEngine::RenderCommandQueue::clear();

        AnEngine::Renderer::beginScene();

        shader->bind();
        AnEngine::Renderer::submit(squareVA);

        AnEngine::Renderer::endScene();

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

private:
    std::shared_ptr<AnEngine::Shader> shader;
    std::shared_ptr<AnEngine::VertexArray> squareVA;
    std::shared_ptr<AnEngine::VertexBuffer> squareVB;
    std::shared_ptr<AnEngine::IndexBuffer> squareIB;
};

class ExampleLayerTwo : public AnEngine::Layer {
public:
    ExampleLayerTwo() : Layer("Example") {}

    void onUpdate() override {
        AE_INFO("ExampleLayerTwo::Update");

        if (AnEngine::Input::isKeyPressed(AE_KEY_BACKSPACE)) {
            AE_ERROR("BACKSPACE is pressed (poll)!");
        }
    }

    virtual void onImGuiRender() override {
        ImGui::Begin("Test2");
        ImGui::Text("Hello World!2");
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
        pushLayer(new ExampleLayerTwo());
    }

    ~Sandbox() {}
};

AnEngine::Application* AnEngine::CreateApplication() {
    RenderAPI::setAPI(RenderAPI::OpenGL);
    return new Sandbox();
}
