#include <entt/entt.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CrankEditor.hpp"

#include "ImGuizmo.h"
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
#include "Panels/ContentBrowserPanel.hpp"
#include "Panels/PropertiesPanel.hpp"
#include "Panels/ScenesPanel.hpp"
#include "Panels/StatisticsPanel.hpp"
#include "Panels/ViewportPanel.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
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
        spec.Attachments = {FrameBufferTexFormat::RGBA8,
                            FrameBufferTexFormat::RED_INTEGER,
                            FrameBufferTexFormat::Depth};
        frameBuffer = FrameBuffer::create(spec);

        activeScene = MakeRef<Scene>("Test Scene");

        CameraSpec camSpec{ProjectionType::Perspective, 30.0f, 1.778f, 0.1f, 1000.0f};
        editorCam = MakeRef<EditorCamera>(camSpec);

        dockSpace = MakeRef<DockSpace>();

        sceneHierarchy = MakeRef<ScenesPanel>("Unnamed Scene", activeScene);
        properties = MakeRef<PropertiesPanel>("Properties", sceneHierarchy);
        viewport = MakeRef<ViewportPanel>("Viewport", frameBuffer, editorCam, dockSpace,
                                          sceneHierarchy, activeScene);
        contentBrowser = MakeRef<ContentBrowserPanel>("Content Browser");
        statistics = MakeRef<StatisticsPanel>("Statistics");

        fileMenu = MakeRef<FileMenu>("File", sceneHierarchy, activeScene, dockSpace);


        dockSpace->addPanel(sceneHierarchy);
        dockSpace->addPanel(properties);
        dockSpace->addPanel(viewport);
        dockSpace->addPanel(contentBrowser);
        dockSpace->addPanel(statistics);

        dockSpace->addMenubarMenu(fileMenu);

        CommandLine cmdLine = Application::getCommandLine();
        if (cmdLine.hasArgs()) {
            std::string sceneFilePath = cmdLine.args[0];
            SceneSerialiser serialiser(activeScene);
            try {
                serialiser.deserialise(sceneFilePath);
            } catch (std::runtime_error& e) {
                AE_CORE_ERROR("Failed to load scene file: {0}", e.what());
            }
        }
    }

    void CrankEditor::onDetach() {}

    void CrankEditor::onUpdate(TimeStep deltaTime) {
        auto vpSize = dockSpace->getViewportSize();

        if (auto fbSpec = frameBuffer->getSpecification();
            vpSize.x > 0.0f && vpSize.y > 0.0f &&
            (fbSpec.Width != vpSize.x || fbSpec.Height != vpSize.y)) {
            frameBuffer->resize((uint32_t)vpSize.x, (uint32_t)vpSize.y);

            editorCam->setViewportSize(vpSize.x, vpSize.y);

            activeScene->onResize((uint32_t)vpSize.x, (uint32_t)vpSize.y);
        }

        // if (dockSpace->isViewportFocused())
        editorCam->onUpdate(deltaTime);

        Renderer2D::resetStats();
        Renderer2D::getStats().lastFrameTime = deltaTime.getMilliseconds();

        frameBuffer->bind();
        RenderCommandQueue::clearColour({0.1f, 0.1f, 0.1f, 1});
        RenderCommandQueue::clear();

        frameBuffer->clearColourAttachment(1, -1);

        activeScene->onUpdateEditor(deltaTime, editorCam);
        // activeScene->onUpdateRuntime(deltaTime);

        auto [mouseX, mouseY] = dockSpace->getMousePosInViewport(true);
        if (dockSpace->isMouseInViewport()) {
            int32_t pixel = frameBuffer->readPixels(1, {mouseX, mouseY}, {1, 1},
                                                    FrameBufferTexFormat::RED_INTEGER)[0];
            hoveredEntity =
                pixel == -1 ? Entity() : Entity{(entt::entity)pixel, activeScene.get()};
            statistics->setHoveredEntity(hoveredEntity);
        }

        frameBuffer->unBind();
    }

    void CrankEditor::onImGuiRender() { dockSpace->render(); }

    void CrankEditor::onEvent(Event& event) {
        editorCam->onEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(CrankEditor::onKeyPressed));
        dispatcher.dispatch<MouseButtonPressedEvent>(
            BIND_EVENT_FN(CrankEditor::OnMouseClick));
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

    bool CrankEditor::OnMouseClick(MouseButtonPressedEvent& mcEvent) {
        if (mcEvent.getMouseButton() == MouseCode::ButtonLeft) {
            if (dockSpace->isMouseInViewport() && !ImGuizmo::IsOver())
                sceneHierarchy->setSelectedEntity(hoveredEntity);
        }

        return true;
    }

}  // namespace AnEngine::Crank
