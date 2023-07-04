#include <AnEngine.hpp>

#include "imgui.h"
#include "imgui_internal.h"


#define UNIFORMS(varName, ...) \
    std::initializer_list<AnEngine::ShaderUniformBase> varName = {__VA_ARGS__}

class ExampleLayer : public AnEngine::Layer {
   public:
    ExampleLayer()
        : Layer("Example"),
          camera(-1.6f, 1.6f, -0.9f, 0.9f),
          cameraPosition(0.0f),
          cameraRotation(0.0f) {
        // rectangle
        {
            squareVA.reset(AnEngine::VertexArray::create());

            float vertices[3 * 4] = {
                -0.75f, -0.75f, 0.0f, 0.75f, -0.75f, 0.0f, 0.75f, 0.75f, 0.0f, -0.75f, 0.75f, 0.0f,
            };
            squareVB.reset(AnEngine::VertexBuffer::create(vertices, sizeof(vertices)));

            AnEngine::BufferLayout layout = {
                {AnEngine::ShaderDataType::Vec3, "position"},
            };
            squareVB->setLayout(layout);

            squareVA->addVertexBuffer(squareVB);

            uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
            squareIB.reset(AnEngine::IndexBuffer::create(indices, 6));

            squareVA->setIndexBuffer(squareIB);


            AnEngine::InputFileStream vertShaderStream("res/shaders/basic.vert");
            AnEngine::InputFileStream fragShaderStream("res/shaders/basic.frag");
            shader.reset(AnEngine::Shader::create(vertShaderStream, fragShaderStream));
        }

        // triangle
        {
            triangleVA.reset(AnEngine::VertexArray::create());

            float vertices2[3 * 3] = {
                -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
            };
            triangleVB.reset(AnEngine::VertexBuffer::create(vertices2, sizeof(vertices2)));

            AnEngine::BufferLayout layout2 = {
                {AnEngine::ShaderDataType::Vec3, "position"},
            };
            triangleVB->setLayout(layout2);

            triangleVA->addVertexBuffer(triangleVB);

            uint32_t indices2[3] = {0, 1, 2};
            triangleIB.reset(AnEngine::IndexBuffer::create(indices2, 3));

            triangleVA->setIndexBuffer(triangleIB);


            AnEngine::InputFileStream vertShaderStream2("res/shaders/basic.vert");
            AnEngine::InputFileStream fragShaderStream2("res/shaders/basic2.frag");
            shader2.reset(AnEngine::Shader::create(vertShaderStream2, fragShaderStream2));
        }
    }

    void onUpdate() override {
        if (AnEngine::Input::isKeyPressed(AE_KEY_A) || AnEngine::Input::isKeyPressed(AE_KEY_LEFT)) {
            cameraPosition.x -= 0.03f;
        } else if (AnEngine::Input::isKeyPressed(AE_KEY_D) ||
                   AnEngine::Input::isKeyPressed(AE_KEY_RIGHT)) {
            cameraPosition.x += 0.03f;
        }

        if (AnEngine::Input::isKeyPressed(AE_KEY_W) || AnEngine::Input::isKeyPressed(AE_KEY_UP)) {
            cameraPosition.y += 0.03f;
        } else if (AnEngine::Input::isKeyPressed(AE_KEY_S) ||
                   AnEngine::Input::isKeyPressed(AE_KEY_DOWN)) {
            cameraPosition.y -= 0.03f;
        }

        if (AnEngine::Input::isKeyPressed(AE_KEY_Q)) {
            cameraRotation += 0.1f;
        } else if (AnEngine::Input::isKeyPressed(AE_KEY_E)) {
            cameraRotation -= 0.1f;
        }

        camera.setPosition(cameraPosition);
        camera.setRotation(cameraRotation);

        AnEngine::RenderCommandQueue::clearColour({0.1f, 0.1f, 0.1f, 1});
        AnEngine::RenderCommandQueue::clear();

        AnEngine::Renderer::beginScene(camera);

        UNIFORMS(uniforms, AnEngine::ShaderUniform("Ucolour", glm::vec4(0.8f, 0.2f, 0.5f, 1.0f)));

        AnEngine::Renderer::submit(shader, squareVA, uniforms);
        AnEngine::Renderer::submit(shader2, triangleVA);

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

    void onEvent(AnEngine::Event& event) override {}

   private:
    std::shared_ptr<AnEngine::Shader> shader;
    std::shared_ptr<AnEngine::VertexArray> squareVA;
    std::shared_ptr<AnEngine::VertexBuffer> squareVB;
    std::shared_ptr<AnEngine::IndexBuffer> squareIB;

    std::shared_ptr<AnEngine::Shader> shader2;
    std::shared_ptr<AnEngine::VertexArray> triangleVA;
    std::shared_ptr<AnEngine::VertexBuffer> triangleVB;
    std::shared_ptr<AnEngine::IndexBuffer> triangleIB;

    AnEngine::OrthographicCamera camera;
    glm::vec3 cameraPosition;
    float cameraRotation;
};

class ExampleLayerTwo : public AnEngine::Layer {
   public:
    ExampleLayerTwo() : Layer("Example") {}

    void onUpdate() override {
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
