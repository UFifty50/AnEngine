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
#include "Events/KeyEvent.hpp"
#include "Menus/Filemenu.hpp"
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
#include "Scene/SceneSerialiser.hpp"
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
                pos.x -= speed * (zoom / 2) * deltaTime;
            } else if (Input::isKeyPressed(KeyCode::D)) {
                pos.x += speed * (zoom / 2) * deltaTime;
            }

            if (Input::isKeyPressed(KeyCode::W)) {
                pos.y += speed * (zoom / 2) * deltaTime;
            } else if (Input::isKeyPressed(KeyCode::S)) {
                pos.y -= speed * (zoom / 2) * deltaTime;
            }
        }
    };

    CrankEditor::CrankEditor() : Layer("CrankEditor") {}

    void CrankEditor::onAttach() {
        Application::loadUILayout("assets/layouts/CrankEditorLayout.ini");

        FrameBufferSpec spec = {1280, 720};
        frameBuffer = FrameBuffer::create(spec);

        activeScene = MakeRef<Scene>("Test Scene");
        // playerEntity = activeScene->createEntity("Player");
        // bgEntity = activeScene->createEntity("Background");
        // cameraEntity = activeScene->createEntity("Camera");
        // lockedCameraEntity = activeScene->createEntity("LockedCamera");

        // playerEntity.addComponent<SpriteRendererComponent>(
        //     glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

        // bgEntity.addComponent<SpriteRendererComponent>(glm::vec4{1.0f, 0.0f,
        // 0.0f, 1.0f});

        // cameraEntity.addComponent<CameraComponent>();
        // auto& cc = lockedCameraEntity.addComponent<CameraComponent>();
        // cc.Primary = false;

        //// cameraEntity.addNativeScript<LockedCameraController>("Camera Controller");
        // cameraEntity.addComponent<NativeScriptComponent>("Camera Controller")
        //     .bind<CameraController>();
        // lockedCameraEntity.addComponent<NativeScriptComponent>("Locked Camera
        // Controller")
        //     .bind<CameraController>();

        dockSpace = MakeRef<DockSpace>();

        sceneHierarchy = MakeRef<ScenesPanel>("Unnamed Scene", activeScene);
        properties = MakeRef<PropertiesPanel>("Properties", sceneHierarchy);
        viewport = MakeRef<ViewportPanel>("Viewport", frameBuffer, dockSpace);
        statistics = MakeRef<StatisticsPanel>("Statistics");

        fileMenu = MakeRef<FileMenu>("File", sceneHierarchy, activeScene, dockSpace);


        dockSpace->addPanel(sceneHierarchy);
        dockSpace->addPanel(properties);
        dockSpace->addPanel(viewport);
        dockSpace->addPanel(statistics);

        dockSpace->addMenubarMenu(fileMenu);
    }

    void CrankEditor::onDetach() {}

    void CrankEditor::onUpdate(TimeStep deltaTime) {
        if (auto fbSpec = frameBuffer->getSpecification();
            dockSpace->getViewportSize().x > 0.0f &&
            dockSpace->getViewportSize().y > 0.0f &&
            (fbSpec.Width != dockSpace->getViewportSize().x ||
             fbSpec.Height != dockSpace->getViewportSize().y)) {
            frameBuffer->resize((uint32_t)dockSpace->getViewportSize().x,
                                (uint32_t)dockSpace->getViewportSize().y);

            activeScene->onResize(dockSpace->getViewportSize().x,
                                  dockSpace->getViewportSize().y);
        }

        Renderer2D::resetStats();
        Renderer2D::getStats().lastFrameTime = deltaTime.getMilliseconds();

        frameBuffer->bind();
        RenderCommandQueue::clearColour({0.1f, 0.1f, 0.1f, 1});
        RenderCommandQueue::clear();

        activeScene->onUpdate(deltaTime);
        frameBuffer->unBind();
    }

    void CrankEditor::onImGuiRender() { dockSpace->render(); }

    void CrankEditor::onEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(CrankEditor::onKeyPressed));
    }

    bool CrankEditor::onKeyPressed(KeyPressedEvent& kpEvent) {
        bool ctrl = Input::isKeyPressed(KeyCode::LeftControl) ||
                    Input::isKeyPressed(KeyCode::RightControl);
        bool shift = Input::isKeyPressed(KeyCode::LeftShift) ||
                     Input::isKeyPressed(KeyCode::RightShift);

        switch (kpEvent.getKeyCode()) {
            case KeyCode::S:
                if (ctrl && shift) FileMenu::SaveScene(activeScene);
                break;

            case KeyCode::N:
                if (ctrl && !shift)
                    FileMenu::NewScene(sceneHierarchy, activeScene, dockSpace);
                break;

            case KeyCode::O:
                if (ctrl && !shift)
                    FileMenu::OpenScene(sceneHierarchy, activeScene, dockSpace);
                break;
        }

        return true;
    }
}  // namespace AnEngine::Crank
