#include <AnEngine.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SandBox2D.hpp"

#include "imgui.h"
#include "imgui_internal.h"


SandBox2D::SandBox2D()
    : Layer("Sandbox2D"), cameraController(1280.0f / 720.0f, 75, true, true) {}

void SandBox2D::onAttach() {
    texture = AnEngine::Texture2D::create("assets/textures/Checkerboard.png");
}

void SandBox2D::onDetach() {}

void SandBox2D::onUpdate(AnEngine::TimeStep deltaTime) {
    cameraController.onUpdate(deltaTime);

    AnEngine::RenderCommandQueue::clearColour({0.1f, 0.1f, 0.1f, 1});
    AnEngine::RenderCommandQueue::clear();

    AnEngine::Renderer2D::beginScene(cameraController.getCamera());

    // UNIFORMS(uniforms, AnEngine::ShaderUniform("Ucolour", squareColour));

    // auto squareShader = shaderLibrary.get("SmallSquares");

    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            AnEngine::Renderer2D::drawQuad({x + 0.3f, y + 0.3f, -0.1f}, {1.0f, 1.0f},
                                           45.0f, squareColour);
            //   AnEngine::Renderer2D::submit(squareShader, squareVA, transform,
            //   uniforms);
        }
    }

    for (int x = 0; x < 19; x++) {
        for (int y = 0; y < 19; y++) {
            AnEngine::Renderer2D::drawQuad({x + 0.8f, y + 0.8f, 0.0f}, {0.5f, 0.5f},
                                           45.0f, squareColour2);
            //   AnEngine::Renderer2D::submit(squareShader, squareVA, transform,
            //   uniforms);
        }
    }

    AnEngine::Renderer2D::drawQuad({10.0f, 10.0f, -0.2f}, {50.0f, 50.0f}, 0.0f, texture);

    AnEngine::Renderer2D::endScene();
}

void SandBox2D::onImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Colour Picker 1", glm::value_ptr(squareColour));
    ImGui::ColorEdit4("Colour Picker 2", glm::value_ptr(squareColour2));
    ImGui::End();
}

void SandBox2D::onEvent(AnEngine::Event& event) { cameraController.onEvent(event); }
