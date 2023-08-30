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
    player = AnEngine::Sprite(texture);


    AnEngine::Particle2D particle1;
    AnEngine::Particle2D particle2;

    {
        particle1.props.velocity = {0.5f, -1.0f, 0.0f};
        particle1.props.startColour = {0.0f, 0.0f, 1.0f, 1.0f};
        particle1.props.endColour = {0.0f, 1.0f, 0.0f, 1.0f};
        particle2.props.startRotation = 45.0f;
        particle2.props.endRotation = 100.0f;
        particle1.props.startSize = 0.7f;
        particle1.props.endSize = 0.0f;
        particle1.props.lifeTime = 5.0f;
    }

    {
        particle2.props.velocity = {0.0f, 0.0f, 0.0f};
        particle2.props.velocityVariation = {3.0f, 1.0f, 0.0f};
        particle2.props.startColour = {1.0f, 0.0f, 0.0f, 1.0f};
        particle2.props.endColour = {0.0f, 0.0f, 1.0f, 1.0f};
        particle2.props.startRotation = 0.0f;
        particle2.props.endRotation = 90.0f;
        particle2.props.startSize = 0.5f;
        particle2.props.endSize = 0.0f;
        particle2.props.lifeTime = 1.0f;
    }

    particle1.init();
    particle2.init();

    particleSpawner += particle1;
    particleSpawner += particle2;

    particleSpawner.reset();
    particleSpawner.enable();
}

void SandBox2D::onDetach() {}

void SandBox2D::onUpdate(AnEngine::TimeStep deltaTime) {
    AE_PROFILE_FUNCTION()

    if (AnEngine::Input::isMouseButtonPressed(AE_MOUSE_BUTTON_RIGHT)) {
        particleSpawner.setSpawnRate(10.0f);
        particleSpawner.setSizeVariation(sizeVariation);

        glm::vec2 mousePos = AnEngine::Input::getMousePosition();
        float winWidth = AnEngine::Application::get().getWindow().getWidth();
        float winHeight = AnEngine::Application::get().getWindow().getHeight();
        auto camBounds = cameraController.getOrthographicCamera()->getBounds();
        glm::vec3 pos = cameraController.getCamera()->getPosition();

        float x =
            (mousePos.x / winWidth) * camBounds.getWidth() - camBounds.getWidth() * 0.5f;
        float y = camBounds.getHeight() * 0.5f -
                  (mousePos.y / winHeight) * camBounds.getHeight();

        particleSpawner.setPosition({x + pos.x, y + pos.y, 0.1f});
    } else {
        particleSpawner.setSpawnRate(0.0f);
    }

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
        AE_PROFILE_SCOPE("Scene")
        /*AnEngine::Renderer2D::beginScene(cameraController.getCamera());

        auto attributes =
            ATTRIBUTE_ARRAY(AnEngine::ShaderUniform("tint", tint),
                            AnEngine::ShaderUniform("tilingFactor", tilingFactor));

        AnEngine::Renderer2D::drawQuad({0.0f, 0.0f, -0.1f}, {50.0f, 50.0f}, 0.0f, texture,
                                       attributes);

        AnEngine::Renderer2D::endScene();*/


        /*AnEngine::Renderer2D::beginScene(cameraController.getCamera());


        for (float x = -20.0f; x < 20.0f; x += 0.5f) {
            for (float y = -20.0f; y < 20.0f; y += 0.5f) {
                glm::vec4 colour = {(y + 5.0f) / 10.0f, 0.4f, (x + 5.0f) / 10.0f, 0.5f};
                AnEngine::Renderer2D::drawQuad({x, y, 0.0f}, {0.45f, 0.45f}, 0.0f,
                                               colour);
            }
        }

        AnEngine::Renderer2D::endScene();*/

        AnEngine::Renderer2D::beginScene(cameraController.getCamera());


        // gravity
        glm::vec3 gravity = {0.0f, -9.8f, 0.0f};

        if (AnEngine::Input::isKeyPressed(AE_KEY_LEFT)) {
            playerX -= 0.03f;
        } else if (AnEngine::Input::isKeyPressed(AE_KEY_RIGHT)) {
            playerX += 0.03f;
        }

        if (playerY > 0.0f) {
            playerY += gravity.y * deltaTime.getSeconds();
        }

        player.render({playerX, playerY, 0.1f}, 0.0f);


        AnEngine::Renderer2D::endScene();


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

void SandBox2D::onEvent(AnEngine::Event& event) {
    cameraController.onEvent(event);
    AnEngine::EventDispatcher dispatcher(event);

    dispatcher.dispatch<AnEngine::KeyPressedEvent>(
        [&](AnEngine::KeyPressedEvent& e) -> bool {
            if (e.getKeyCode() == AE_KEY_UP && playerY <= 0.1f) {
                playerY += 5.00f;
                return true;
            }
            return false;
        });

    /*dispatcher.dispatch<AnEngine::MouseButtonPressedEvent>(
        [&](AnEngine::MouseButtonPressedEvent& e) -> bool {
            if (e.getMouseButton() == AE_MOUSE_BUTTON_RIGHT)
                particleSpawner.setSpawnRate(toggle ? 50.0f : 0.0f);
            toggle = !toggle;
            return false;
        });*/
}
