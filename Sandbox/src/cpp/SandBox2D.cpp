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
    AE_PROFILE_FUNCTION()

    {
        AE_PROFILE_SCOPE("CameraController::onUpdate")
        cameraController.onUpdate(deltaTime);
    }

    {
        AE_PROFILE_SCOPE("Render Commands")
        AnEngine::RenderCommandQueue::clearColour({0.1f, 0.1f, 0.1f, 1});
        AnEngine::RenderCommandQueue::clear();
    }

    {
        AE_PROFILE_SCOPE("Scene")
        AnEngine::Renderer2D::beginScene(cameraController.getCamera());

        for (int x = 0; x < 20; x++) {
            for (int y = 0; y < 20; y++) {
                AnEngine::Renderer2D::drawQuad({x + 0.5f, y + 0.5f, 0.1f}, {1.0f, 1.0f},
                                               squareRotation, squareColour);
            }
        }

        for (int x = 0; x < 19; x++) {
            for (int y = 0; y < 19; y++) {
                AnEngine::Renderer2D::drawQuad({x + 1.2f, y + 1.2f, 0.0f}, {0.5f, 0.5f},
                                               squareRotation2, squareColour2);
            }
        }

        auto attributes =
            ATTRIBUTE_ARRAY(AnEngine::ShaderUniform("tint", tint),
                            AnEngine::ShaderUniform("tilingFactor", tilingFactor));

        AnEngine::Renderer2D::drawQuad({25.0f, 25.0f, -0.1f}, {10.0f, 10.0f}, 0.0f,
                                       texture, attributes);

        AnEngine::Renderer2D::endScene();
    }
}

void SandBox2D::onImGuiRender() {
    AE_PROFILE_FUNCTION()
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Colour Picker 1", glm::value_ptr(squareColour));
    ImGui::ColorEdit4("Colour Picker 2", glm::value_ptr(squareColour2));
    ImGui::ColorEdit4("Tint", glm::value_ptr(tint));
    ImGui::DragFloat("Rotation 1", &squareRotation, 0.1f, 0.0f, 180.0f);
    ImGui::DragFloat("Rotation 2", &squareRotation2, 0.1f, 0.0f, 180.0f);
    ImGui::DragFloat("Tiling Factor", &tilingFactor, 0.1f, 0.0f, 10.0f);
    ImGui::End();

    PROFILE_UI()
}

void SandBox2D::onEvent(AnEngine::Event& event) { cameraController.onEvent(event); }
