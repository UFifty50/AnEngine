#include <AnEngine.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "imgui.h"
#include "imgui_internal.h"


#define UNIFORMS(varName, ...) \
    AnEngine::ShaderUniformVector varName = {__VA_ARGS__}

class ExampleLayer : public AnEngine::Layer {
public:
    ExampleLayer()
        : Layer("Example"),
          camera(-1.6f, 1.6f, -0.9f, 0.9f),
          cameraPosition(0.0f),
          cameraRotation(0.0f),
          objPosition(0.0f) {
        // square
        {
            squareVA.reset(AnEngine::VertexArray::create());

            // clang-format off
            float vertices[3 * 4] = {
                -0.5f, -0.5f, 0.0f,
                 0.5f, -0.5f, 0.0f,
                 0.5f,  0.5f, 0.0f,
                -0.5f,  0.5f, 0.0f,
            };
            // clang-format on

            squareVB.reset(
                AnEngine::VertexBuffer::create(vertices, sizeof(vertices)));

            AnEngine::BufferLayout layout = {
                {AnEngine::ShaderDataType::Vec3, "position"},
            };
            squareVB->setLayout(layout);

            squareVA->addVertexBuffer(squareVB);

            uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
            squareIB.reset(AnEngine::IndexBuffer::create(indices, 6));

            squareVA->setIndexBuffer(squareIB);

            AnEngine::InputFileStream vertShaderStream(
                "res/shaders/basic.vert");
            AnEngine::InputFileStream fragShaderStream(
                "res/shaders/basic.frag");
            shader.reset(
                AnEngine::Shader::create(vertShaderStream, fragShaderStream));
        }

        // triangle
        {
            triangleVA.reset(AnEngine::VertexArray::create());

            float vertices2[3 * 3] = {
                -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
            };
            triangleVB.reset(
                AnEngine::VertexBuffer::create(vertices2, sizeof(vertices2)));

            AnEngine::BufferLayout layout2 = {
                {AnEngine::ShaderDataType::Vec3, "position"},
            };
            triangleVB->setLayout(layout2);

            triangleVA->addVertexBuffer(triangleVB);

            uint32_t indices2[3] = {0, 1, 2};
            triangleIB.reset(AnEngine::IndexBuffer::create(indices2, 3));

            triangleVA->setIndexBuffer(triangleIB);

            AnEngine::InputFileStream vertShaderStream2(
                "res/shaders/basic.vert");
            AnEngine::InputFileStream fragShaderStream2(
                "res/shaders/basic2.frag");
            shader2.reset(
                AnEngine::Shader::create(vertShaderStream2, fragShaderStream2));
        }
    }

    void onUpdate(AnEngine::TimeStep deltaTime) override {
        if (AnEngine::Input::isKeyPressed(AE_KEY_A)) {
            cameraPosition.x -= 1.0f * deltaTime;
        } else if (AnEngine::Input::isKeyPressed(AE_KEY_D)) {
            cameraPosition.x += 1.0f * deltaTime;
        }

        if (AnEngine::Input::isKeyPressed(AE_KEY_W)) {
            cameraPosition.y += 1.0f * deltaTime;
        } else if (AnEngine::Input::isKeyPressed(AE_KEY_S)) {
            cameraPosition.y -= 1.0f * deltaTime;
        }

        if (AnEngine::Input::isKeyPressed(AE_KEY_Q)) {
            cameraRotation += 10.0f * deltaTime;
        } else if (AnEngine::Input::isKeyPressed(AE_KEY_E)) {
            cameraRotation -= 10.0f * deltaTime;
        }

        camera.setPosition(cameraPosition);
        camera.setRotation(cameraRotation);

        AnEngine::RenderCommandQueue::clearColour({0.1f, 0.1f, 0.1f, 1});
        AnEngine::RenderCommandQueue::clear();

        AnEngine::Renderer::beginScene(camera);


        static const glm::mat4 scale =
            glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        glm::vec4 red(0.8f, 0.2f, 0.3f, 1.0f);
        glm::vec4 green(0.3f, 0.8f, 0.2f, 1.0f);
        glm::vec4 blue(0.2f, 0.3f, 0.8f, 0.0f);

        UNIFORMS(uniforms, AnEngine::ShaderUniform(
                               "Ucolour", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));

        for (int x = 0; x < 20; x++) {
            for (int y = 0; y < 20; y++) {
                float nx = x * 0.05f;
                float ny = y * 0.125f;

                uniforms["Ucolour"] =
                    (x + y) % 2 == 0
                        ? red * glm::vec4(nx, ny, nx / 2 + ny / 2, 1.0f)
                        : blue * glm::vec4(nx, ny, nx / 2 + ny / 2, 1.0f);
                glm::vec3 pos =
                    glm::vec3(x * 0.11f + 0.3f, y * 0.11f + 0.2f, 0.0f);
                glm::mat4 transform =
                    glm::translate(glm::mat4(1.0f), pos) * scale;
                AnEngine::Renderer::submit(shader, squareVA, transform,
                                           uniforms);
            }
        }

        UNIFORMS(triangleUniforms, AnEngine::ShaderUniform("Ucolour", red));

        AnEngine::Renderer::submit(shader2, triangleVA, glm::mat4(1.0f),
                                   triangleUniforms);

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
    AnEngine::Ref<AnEngine::Shader> shader;
    AnEngine::Ref<AnEngine::VertexArray> squareVA;
    AnEngine::Ref<AnEngine::VertexBuffer> squareVB;
    AnEngine::Ref<AnEngine::IndexBuffer> squareIB;

    AnEngine::Ref<AnEngine::Shader> shader2;
    AnEngine::Ref<AnEngine::VertexArray> triangleVA;
    AnEngine::Ref<AnEngine::VertexBuffer> triangleVB;
    AnEngine::Ref<AnEngine::IndexBuffer> triangleIB;

    AnEngine::OrthographicCamera camera;
    glm::vec3 cameraPosition;
    float cameraRotation;

    glm::vec3 objPosition;
};

class ExampleLayerTwo : public AnEngine::Layer {
public:
    ExampleLayerTwo() : Layer("Example") {}

    void onUpdate(AnEngine::TimeStep deltaTime) override {
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
