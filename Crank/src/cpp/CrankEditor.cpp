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
#include "Panels/ScenesPanel.hpp"
#include "Panels/SettingsPanel.hpp"
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
            auto& transform = getComponent<TransformComponent>().Transform;
            transform[3][0] = Random::getFloat() * 10.0f - 5.0f;
        }

        void onUpdate(TimeStep deltaTime) {
            auto& transform = getComponent<TransformComponent>().Transform;
            float speed = 2.0f;
            float zoom = 2.0f;

            if (Input::isKeyPressed(KeyCode::A)) {
                transform[3][0] -= speed * (zoom / 2) * deltaTime;
            } else if (Input::isKeyPressed(KeyCode::D)) {
                transform[3][0] += speed * (zoom / 2) * deltaTime;
            }

            if (Input::isKeyPressed(KeyCode::W)) {
                transform[3][1] += speed * (zoom / 2) * deltaTime;
            } else if (Input::isKeyPressed(KeyCode::S)) {
                transform[3][1] -= speed * (zoom / 2) * deltaTime;
            }
        }
    };

    CrankEditor::CrankEditor() : Layer("CrankEditor") {}

    void CrankEditor::onAttach() {
        Application::loadUILayout("assets/layouts/CrankEditorLayout.ini");

        activeScene = MakeRef<Scene>();
        playerEntity = activeScene->createEntity("Player");
        cameraEntity = activeScene->createEntity("Camera");
        lockedCameraEntity = activeScene->createEntity("LockedCamera");

        playerEntity.addComponent<SpriteRendererComponent>(
            glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

        cameraEntity.addComponent<CameraComponent>();
        auto& cc = lockedCameraEntity.addComponent<CameraComponent>();
        cc.Primary = false;

        // cameraEntity.addNativeScript<LockedCameraController>("Camera Controller");
        cameraEntity.addComponent<NativeScriptComponent>("Camera Controller")
            .bind<CameraController>();
        lockedCameraEntity.addComponent<NativeScriptComponent>("Locked Camera Controller")
            .bind<CameraController>();

        //   lockedCameraEntity.removeComponent<CameraComponent>();


        FrameBufferSpec spec = {1280, 720};
        frameBuffer = FrameBuffer::create(spec);


        dockSpace.addPanel(MakeRef<ViewportPanel>("viewport", frameBuffer, dockSpace));
        dockSpace.addPanel(MakeRef<ScenesPanel>("Scene Heirarchy"));
        dockSpace.addPanel(MakeRef<SettingsPanel>("Settings", playerEntity, cameraEntity,
                                                  lockedCameraEntity, CameraA));
        dockSpace.addPanel(MakeRef<StatisticsPanel>("Statistics"));
    }

    void CrankEditor::onDetach() {}

    void CrankEditor::onUpdate(TimeStep deltaTime) {
        //     if (viewportFocused) cameraController.onUpdate(deltaTime);

        if (dockSpace.getViewportSize().x > 0.0f &&
            dockSpace.getViewportSize().y > 0.0f) {
            frameBuffer->resize((uint32_t)dockSpace.getViewportSize().x,
                                (uint32_t)dockSpace.getViewportSize().y);
            //    cameraController.onResize(viewportSize.x, viewportSize.y);

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
