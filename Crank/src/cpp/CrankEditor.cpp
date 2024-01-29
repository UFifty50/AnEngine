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
#include "Renderer/Renderer3D.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Scene2D.hpp"
#include "Scene/Scene3D.hpp"
#include "Scene/SceneSerialiser.hpp"
#include "Scene/ScriptableEntity.hpp"


namespace AnEngine::Crank {
    Ref<Scene> g_ActiveScene;

    Ref<DockSpace> g_DockSpace;

    Ref<ScenesPanel> gPanel_SceneHierarchy;
    Ref<PropertiesPanel> gPanel_Properties;
    Ref<ViewportPanel> gPanel_Viewport;
    Ref<ContentBrowserPanel> gPanel_ContentBrowser;
    Ref<StatisticsPanel> gPanel_Statistics;

    Ref<FileMenu> gMenu_FileOps;

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

        g_ActiveScene = MakeRef<Scene2D>("Test Scene");

        CameraSpec2D camSpec{30.0f, 1.778f, 0.1f, 1000.0f};
        editorCam3D = MakeRef<EditorCamera2D>(camSpec);

        g_DockSpace = MakeRef<DockSpace>();

        gPanel_SceneHierarchy = MakeRef<ScenesPanel>("Unnamed Scene");
        gPanel_Properties = MakeRef<PropertiesPanel>("Properties");
        gPanel_Viewport = MakeRef<ViewportPanel>("Viewport", frameBuffer, editorCam3D);
        gPanel_ContentBrowser = MakeRef<ContentBrowserPanel>("Content Browser");
        gPanel_Statistics = MakeRef<StatisticsPanel>("Statistics");

        gMenu_FileOps = MakeRef<FileMenu>("File");


        g_DockSpace->addPanel(gPanel_SceneHierarchy);
        g_DockSpace->addPanel(gPanel_Properties);
        g_DockSpace->addPanel(gPanel_Viewport);
        g_DockSpace->addPanel(gPanel_ContentBrowser);
        g_DockSpace->addPanel(gPanel_Statistics);

        g_DockSpace->addMenubarMenu(gMenu_FileOps);


        CommandLine cmdLine = Application::getCommandLine();
        if (cmdLine.hasArgs()) {
            std::string sceneFilePath = cmdLine.args[0];
            SceneSerialiser serialiser(g_ActiveScene);
            try {
                serialiser.deserialise(sceneFilePath);
            } catch (std::runtime_error& e) {
                AE_CORE_ERROR("Failed to load scene file: {0}", e.what());
            }
        }
    }

    void CrankEditor::onDetach() {}

    void CrankEditor::onUpdate(TimeStep deltaTime) {
        auto vpSize = g_DockSpace->getViewportSize();

        if (auto fbSpec = frameBuffer->getSpecification();
            vpSize.x > 0.0f && vpSize.y > 0.0f &&
            (fbSpec.Width != vpSize.x || fbSpec.Height != vpSize.y)) {
            frameBuffer->resize((uint32_t)vpSize.x, (uint32_t)vpSize.y);

            editorCam3D->setViewportSize(vpSize.x, vpSize.y);

            g_ActiveScene->onResize((uint32_t)vpSize.x, (uint32_t)vpSize.y);
        }

        // if (dockSpace->isViewportFocused())
        editorCam3D->onUpdate(deltaTime);

        Renderer3D::resetStats();
        Renderer3D::getStats().lastFrameTime = deltaTime.getMilliseconds();

        frameBuffer->bind();
        RenderCommandQueue::clearColour({0.1f, 0.1f, 0.1f, 1.0f});
        RenderCommandQueue::clear();

        frameBuffer->clearColourAttachment(1, -1);

        g_ActiveScene->onUpdateEditor(deltaTime, editorCam3D);
        // activeScene->onUpdateRuntime(deltaTime);

        auto [mouseX, mouseY] = g_DockSpace->getMousePosInViewport(true);
        if (g_DockSpace->isMouseInViewport()) {
            int32_t pixel = frameBuffer->readPixels(1, {mouseX, mouseY}, {1, 1},
                                                    FrameBufferTexFormat::RED_INTEGER)[0];
            hoveredEntity =
                pixel == -1 ? Entity() : Entity{(entt::entity)pixel, g_ActiveScene.get()};
            gPanel_Statistics->setHoveredEntity(hoveredEntity);
        }

        frameBuffer->unBind();
    }

    void CrankEditor::onImGuiRender() { g_DockSpace->render(); }

    void CrankEditor::onEvent(Event& event) {
        editorCam3D->onEvent(event);

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
                if (ctrl && shift) FileMenu::SaveActiveScene();
                break;

            case KeyCode::N:
                if (ctrl && !shift) FileMenu::NewScene();
                break;

            case KeyCode::O:
                if (ctrl && !shift)
                    if (!FileMenu::OpenSceneMenu())
                        AE_CORE_ERROR("Failed to open scene.");
                break;
        }

        return true;
    }

    bool CrankEditor::OnMouseClick(MouseButtonPressedEvent& mcEvent) {
        if (mcEvent.getMouseButton() == MouseCode::ButtonLeft) {
            if (g_DockSpace->isMouseInViewport() && !ImGuizmo::IsOver())
                gPanel_SceneHierarchy->setSelectedEntity(hoveredEntity);
        }

        return true;
    }

}  // namespace AnEngine::Crank
