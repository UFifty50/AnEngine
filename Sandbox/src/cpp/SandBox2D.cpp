#include <AnEngine.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SandBox2D.hpp"

#include "imgui.h"
#include "imgui_internal.h"


SandBox2D::SandBox2D()
    : Layer("Sandbox2D"),
      cameraController(1280.0f / 720.0f, 75, true, true),
      particleSpawner({0.0f, 0.0f, 0.1f}) {}

void SandBox2D::onAttach() {
    texture = AnEngine::Texture2D::create("assets/textures/Checkerboard.png");


    AnEngine::Particle2D particle;
    particle.props.velocity = {0.2f, -1.0f, 0.0f};
    particle.props.startColour = {0.0f, 0.0f, 1.0f, 1.0f};
    particle.props.endColour = {0.0f, 1.0f, 0.0f, 1.0f};
    particle.props.startSize = 1.0f;
    particle.props.endSize = 0.0f;
    particle.props.lifeTime = 2.0f;

    particle.init();

    particleSpawner += particle;

    particleSpawner.reset();
    particleSpawner.enable();
}

void SandBox2D::onDetach() {}

void SandBox2D::onUpdate(AnEngine::TimeStep deltaTime) {
    AE_PROFILE_FUNCTION()

    particleSpawner.setSizeVariation(sizeVariation);
    particleSpawner.setSpawnRate(spawnRate);

    glm::vec2 mousePos = AnEngine::Input::getMousePosition();
    float winWidth = AnEngine::Application::get().getWindow().getWidth();
    float winHeight = AnEngine::Application::get().getWindow().getHeight();
    auto camBounds = cameraController.getCamera()->asOrthographic()->getBounds();
    glm::vec3 pos = cameraController.getCamera()->getPosition();

    float x =
        (mousePos.x / winWidth) * camBounds.getWidth() - camBounds.getWidth() * 0.5f;
    float y =
        camBounds.getHeight() * 0.5f - (mousePos.y / winHeight) * camBounds.getHeight();

    particleSpawner.setPosition({x + pos.x, y + pos.y, 0.1f});

    {
        AE_PROFILE_SCOPE("Camera")
        cameraController.onUpdate(deltaTime);
    }

    {
        AE_PROFILE_SCOPE("Particles")
        particleSpawner.onUpdate(deltaTime);
    }

    AnEngine::Renderer2D::resetStats();
    AnEngine::Renderer2D::getStats().lastFrameTime = deltaTime;
    AE_CORE_INFO("FrameTime: {0}     FPS: {1}", deltaTime.getMilliseconds(),
                 1.0f / deltaTime);

    {
        AE_PROFILE_SCOPE("Render Commands")
        AnEngine::RenderCommandQueue::clearColour({0.1f, 0.1f, 0.1f, 1});
        AnEngine::RenderCommandQueue::clear();
    }

    {
        // AE_PROFILE_SCOPE("Scene")
        // AnEngine::Renderer2D::beginScene(cameraController.getCamera());

        // auto attributes =
        //     ATTRIBUTE_ARRAY(AnEngine::ShaderUniform("tint", tint),
        //                     AnEngine::ShaderUniform("tilingFactor", tilingFactor));

        // AnEngine::Renderer2D::drawQuad({0.0f, 0.0f, -0.1f}, {50.0f, 50.0f}, 0.0f,
        // texture,
        //                                attributes);

        ///*AnEngine::Renderer2D::endScene();


        /*AnEngine::Renderer2D::beginScene(cameraController.getCamera());


        for (float x = -20.0f; x < 20.0f; x += 0.5f) {
            for (float y = -20.0f; y < 20.0f; y += 0.5f) {
                glm::vec4 colour = {(y + 5.0f) / 10.0f, 0.4f, (x + 5.0f) / 10.0f, 0.5f};
                AnEngine::Renderer2D::drawQuad({x, y, 0.0f}, {0.45f, 0.45f}, 0.0f,
                                               colour);
            }
        }

        AnEngine::Renderer2D::endScene();*/

        particleSpawner.emit(cameraController.getCamera());
    }
}

void SandBox2D::onImGuiRender() {
    AE_PROFILE_FUNCTION()
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Tint", glm::value_ptr(tint));
    ImGui::DragFloat("Tiling Factor", &tilingFactor, 0.1f, 0.0f, 10.0f);
    ImGui::DragFloat("Spawn Rate", &spawnRate, 0.1f, 0.0f, 100.0f);
    ImGui::DragFloat("Size Variation", &sizeVariation, 0.1f, 0.0f, 10.0f);
    if (ImGui::Button("Enable Particles")) {
        particleSpawner.enable();
    }

    if (ImGui::Button("Disable Particles")) {
        particleSpawner.disable();
    }

    if (ImGui::Button("Reset Particles")) {
        particleSpawner.reset();
    }

    ImGui::End();

    ImGui::Begin("Statistics");
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Frametime: %fms", (float)AnEngine::Renderer2D::getStats().lastFrameTime);
    ImGui::Text("FPS: %.1fms",
                1.0f / (AnEngine::Renderer2D::getStats().lastFrameTime / 1000.0f));
    ImGui::Text("Draw Calls: %d", AnEngine::Renderer2D::getStats().draws);
    ImGui::Text("Quads: %d", AnEngine::Renderer2D::getStats().quadCount);
    ImGui::Text("Vertices: %d", AnEngine::Renderer2D::getStats().getTotalVertexCount());
    ImGui::Text("Indices: %d", AnEngine::Renderer2D::getStats().getTotalIndexCount());
    ImGui::End();

    PROFILE_UI()
}

void SandBox2D::onEvent(AnEngine::Event& event) { cameraController.onEvent(event); }
