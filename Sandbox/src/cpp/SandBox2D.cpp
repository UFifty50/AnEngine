#include <AnEngine.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SandBox2D.hpp"

#include "imgui.h"
#include "imgui_internal.h"


SandBox2D::SandBox2D()
    : Layer("Sandbox2D"), cameraController(1280.0f / 720.0f, 75, true, true) {}

void SandBox2D::onAttach() {
    // square
    {
        squareVA = AnEngine::VertexArray::create();

        // clang-format off
            float vertices[5 * 4] = {
                -0.5f, -0.5f, 0.0f,
                 0.5f, -0.5f, 0.0f,
                 0.5f,  0.5f, 0.0f,
                -0.5f,  0.5f, 0.0f,
            };
        // clang-format on

        squareVB = AnEngine::VertexBuffer::create(vertices, sizeof(vertices));

        AnEngine::BufferLayout layout = {
            {AnEngine::ShaderDataType::Vec3, "position"},
        };
        squareVB->setLayout(layout);

        squareVA->addVertexBuffer(squareVB);

        uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
        squareIB = AnEngine::IndexBuffer::create(indices, 6);

        squareVA->setIndexBuffer(squareIB);


        auto shader = shaderLibrary.load("SmallSquares", "assets/shaders/basic.glsl");
    }
}

void SandBox2D::onDetach() {}

void SandBox2D::onUpdate(AnEngine::TimeStep deltaTime) {
    cameraController.onUpdate(deltaTime);

    AnEngine::RenderCommandQueue::clearColour({0.1f, 0.1f, 0.1f, 1});
    AnEngine::RenderCommandQueue::clear();

    AnEngine::Renderer2D::beginScene(cameraController.getCamera());

    static const glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    glm::vec4 red(0.8f, 0.2f, 0.3f, 1.0f);
    glm::vec4 green(0.3f, 0.8f, 0.2f, 1.0f);
    glm::vec4 blue(0.2f, 0.3f, 0.8f, 0.0f);

    UNIFORMS(uniforms, AnEngine::ShaderUniform("Ucolour", squareColour));

    auto squareShader = shaderLibrary.get("SmallSquares");

    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            glm::vec3 pos = glm::vec3(x * 0.11f + 0.3f, y * 0.11f + 0.2f, 0.0f);
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
            AnEngine::Renderer2D::submit(squareShader, squareVA, transform, uniforms);
        }
    }

    AnEngine::Renderer2D::endScene();
}

void SandBox2D::onImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Colour Picker", glm::value_ptr(squareColour));
    ImGui::End();
}

void SandBox2D::onEvent(AnEngine::Event& event) { cameraController.onEvent(event); }
