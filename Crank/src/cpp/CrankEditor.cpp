#include <entt/entt.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CrankEditor.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include "Application.hpp"
#include "Core/Core.hpp"
#include "Core/Input.hpp"
#include "Core/Layer.hpp"
#include "Core/Log.hpp"
#include "Core/Random.hpp"
#include "Panels/PropertiesPanel.hpp"
#include "Panels/ScenesPanel.hpp"
#include "Panels/StatisticsPanel.hpp"
#include "Panels/ViewportPanel.hpp"
#include "Renderer/FrameBuffer.hpp"
#include "Renderer/RenderCommandQueue.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Scene.hpp"
#include "Scene/ScriptableEntity.hpp"


namespace AnEngine::Crank {
    class CameraController : public ScriptableEntity {
    public:
        void onCreate() {
            auto& pos = getComponent<TransformComponent>().Position;
            pos.x = Random::getFloat() * 10.0f - 5.0f;
            pos.y = Random::getFloat() * 10.0f - 5.0f;
        }

        void onUpdate(TimeStep deltaTime) {
            auto& pos = getComponent<TransformComponent>().Position;
            float speed = 2.0f;
            float zoom = 2.0f;

            if (Input::isKeyPressed(KeyCode::A)) {
                pos -= speed * (zoom / 2) * deltaTime;
            } else if (Input::isKeyPressed(KeyCode::D)) {
                pos += speed * (zoom / 2) * deltaTime;
            }

            if (Input::isKeyPressed(KeyCode::W)) {
                pos += speed * (zoom / 2) * deltaTime;
            } else if (Input::isKeyPressed(KeyCode::S)) {
                pos -= speed * (zoom / 2) * deltaTime;
            }
        }
    };

    CrankEditor::CrankEditor() : Layer("CrankEditor") {}

    void CrankEditor::onAttach() {
        Application::loadUILayout("assets/layouts/CrankEditorLayout.ini");

        activeScene = MakeRef<Scene>("Test Scene");
        playerEntity = activeScene->createEntity("Player");
        bgEntity = activeScene->createEntity("Background");
        cameraEntity = activeScene->createEntity("Camera");
        lockedCameraEntity = activeScene->createEntity("LockedCamera");

        playerEntity.addComponent<SpriteRendererComponent>(
            glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

        bgEntity.addComponent<SpriteRendererComponent>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});

        cameraEntity.addComponent<CameraComponent>();
        auto& cc = lockedCameraEntity.addComponent<CameraComponent>();
        cc.Primary = false;

        // cameraEntity.addNativeScript<LockedCameraController>("Camera Controller");
        cameraEntity.addComponent<NativeScriptComponent>("Camera Controller")
            .bind<CameraController>();
        lockedCameraEntity.addComponent<NativeScriptComponent>("Locked Camera Controller")
            .bind<CameraController>();


        FrameBufferSpec spec = {1280, 720};
        frameBuffer = FrameBuffer::create(spec);

        auto sp = MakeRef<ScenesPanel>("Scene Heirarchy", activeScene);
        dockSpace.addPanel(sp);
        dockSpace.addPanel(MakeRef<PropertiesPanel>("Properties", sp));
        dockSpace.addPanel(MakeRef<ViewportPanel>("viewport", frameBuffer, dockSpace));
        dockSpace.addPanel(MakeRef<StatisticsPanel>("Statistics"));
    }

    void CrankEditor::onDetach() {}

    void CrankEditor::onUpdate(TimeStep deltaTime) {
        if (dockSpace.getViewportSize().x > 0.0f &&
            dockSpace.getViewportSize().y > 0.0f) {
            frameBuffer->resize((uint32_t)dockSpace.getViewportSize().x,
                                (uint32_t)dockSpace.getViewportSize().y);

            activeScene->onResize(dockSpace.getViewportSize().x,
                                  dockSpace.getViewportSize().y);
        }

        Renderer2D::resetStats();
        Renderer2D::getStats().lastFrameTime = deltaTime.getMilliseconds();

        {
            frameBuffer->bind();
            RenderCommandQueue::clearColour({0.1f, 0.1f, 0.1f, 1});
            RenderCommandQueue::clear();
        }

        {
            activeScene->onUpdate(deltaTime);
            frameBuffer->unBind();
        }
    }

    void CrankEditor::onImGuiRender() { dockSpace.render(); }

    void CrankEditor::onEvent(Event& event) {}
}  // namespace AnEngine::Crank
