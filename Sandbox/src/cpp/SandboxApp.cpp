#include <AnEngine.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "imgui.h"
#include "imgui_internal.h"


#define UNIFORMS(varName, ...) AnEngine::ShaderUniformVector varName = {__VA_ARGS__}

class ExampleLayer : public AnEngine::Layer {
public:
    ExampleLayer()
        : Layer("Example"),
          camera(-1.6f, 1.6f, -0.9f, 0.9f),
          cameraPosition(0.0f),
          cameraRotation(0.0f) {
        // square
        {
            squareVA = AnEngine::VertexArray::create();

            // clang-format off
            float vertices[5 * 4] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
            };
            // clang-format on

            squareVB = AnEngine::VertexBuffer::create(vertices, sizeof(vertices));

            AnEngine::BufferLayout layout = {
                {AnEngine::ShaderDataType::Vec3, "position"},
                {AnEngine::ShaderDataType::Float2, "texCoord"},
            };
            squareVB->setLayout(layout);

            squareVA->addVertexBuffer(squareVB);

            uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
            squareIB = AnEngine::IndexBuffer::create(indices, 6);

            squareVA->setIndexBuffer(squareIB);

            shader = AnEngine::Shader::create("assets/shaders/basic.vert",
                                              "assets/shaders/basic.frag");

            textureShader =
                AnEngine::Shader::create("assets/shaders/mixedTextureShader.glsl");

            texture = AnEngine::Texture2D::create("assets/textures/Checkerboard.png");
            logoTexture = AnEngine::Texture2D::create("assets/textures/ChernoLogo.png");

            textureShader->uploadUniform("Utexture", AnEngine::Sampler2D{0});
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


        static const glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        glm::vec4 red(0.8f, 0.2f, 0.3f, 1.0f);
        glm::vec4 green(0.3f, 0.8f, 0.2f, 1.0f);
        glm::vec4 blue(0.2f, 0.3f, 0.8f, 0.0f);

        UNIFORMS(uniforms, AnEngine::ShaderUniform("Ucolour", squareColour));

        for (int x = 0; x < 20; x++) {
            for (int y = 0; y < 20; y++) {
                // float nx = x * 0.05f;
                // float ny = y * 0.125f;

                // uniforms["Ucolour"] =
                //     (x + y) % 2 == 0
                //         ? red * glm::vec4(nx, ny, nx / 2 + ny / 2, 1.0f)
                //         : blue * glm::vec4(nx, ny, nx / 2 + ny / 2, 1.0f);
                glm::vec3 pos = glm::vec3(x * 0.11f + 0.3f, y * 0.11f + 0.2f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                AnEngine::Renderer::submit(shader, squareVA, transform, uniforms);
            }
        }

        texture->bind();
        AnEngine::Renderer::submit(textureShader, squareVA,
                                   glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        logoTexture->bind();
        AnEngine::Renderer::submit(textureShader, squareVA,
                                   glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        AnEngine::Renderer::endScene();

        if (AnEngine::Input::isKeyPressed(AE_KEY_ENTER)) {
            AE_ERROR("ENTER is pressed (poll)!");
        }
    }

    virtual void onImGuiRender() override {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Colour Picker", glm::value_ptr(squareColour));
        ImGui::End();
    }

    void onEvent(AnEngine::Event& event) override {}

private:
    AnEngine::Ref<AnEngine::Shader> shader, textureShader;
    AnEngine::Ref<AnEngine::VertexArray> squareVA;
    AnEngine::Ref<AnEngine::VertexBuffer> squareVB;
    AnEngine::Ref<AnEngine::IndexBuffer> squareIB;

    AnEngine::Ref<AnEngine::Texture2D> texture, logoTexture;

    AnEngine::OrthographicCamera camera;
    glm::vec3 cameraPosition;
    float cameraRotation;

    glm::vec3 squareColour = {0.2f, 0.3f, 0.8f};
};

class Sandbox : public AnEngine::Application {
public:
    Sandbox() { pushLayer(std::make_shared<ExampleLayer>()); }

    ~Sandbox() {}
};

AnEngine::Application* AnEngine::CreateApplication() {
    RenderAPI::setAPI(RenderAPI::OpenGL);
    return new Sandbox();
}
