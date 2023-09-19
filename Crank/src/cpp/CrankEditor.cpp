#include <entt/entt.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CrankEditor.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include "Core/Core.hpp"
#include "Core/Input.hpp"
#include "Core/Layer.hpp"
#include "Renderer/FrameBuffer.hpp"
#include "Renderer/RenderCommandQueue.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Scene.hpp"
#include "Scene/ScriptableEntity.hpp"


namespace AnEngine {
    class LockedCameraController : public ScriptableEntity {
    public:
        void onCreate() {}
        void onDestroy() {}
        void onUpdate(TimeStep deltaTime) {
            auto& transform = getComponent<TransformComponent>().Transform;
            float speed = 2.0f;
            float zoom = 2.0f;

            if (Input::isKeyPressed(KeyCode::A)) {
                transform[3][0] += speed * (zoom / 2) * deltaTime;
            } else if (Input::isKeyPressed(KeyCode::D)) {
                transform[3][0] -= speed * (zoom / 2) * deltaTime;
            }

            if (Input::isKeyPressed(KeyCode::W)) {
                transform[3][1] -= speed * (zoom / 2) * deltaTime;
            } else if (Input::isKeyPressed(KeyCode::S)) {
                transform[3][1] += speed * (zoom / 2) * deltaTime;
            }
        }
    };

    CrankEditor::CrankEditor()
        : Layer("CrankEditor"),
          cameraController(1280.0f / 720.0f, 75, true, true),
          viewportSize(0.0f),
          viewportPos(0.0f),
          mousePosInViewport(0.0f),
          sheet("assets/textures/RPGpack_sheet_2X.png", 128, 128) {}

    void CrankEditor::onAttach() {
        Application::loadUILayout("assets/layouts/CrankEditorLayout.ini");

        sprite1 = sheet.getSprite({4, 9});

        activeScene = MakeRef<Scene>();
        playerEntity = activeScene->createEntity("Player");
        cameraEntity = activeScene->createEntity("Camera");
        lockedCameraEntity = activeScene->createEntity("LockedCamera");

        playerEntity.addComponent<SpriteRendererComponent>(
            glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

        cameraEntity.addComponent<CameraComponent>();
        auto& cc = lockedCameraEntity.addComponent<CameraComponent>();
        cc.Primary = false;

        // lockedCameraEntity.addNativeScript<LockedCameraController>();
        cameraEntity.addComponent<NativeScriptComponent>().bind<LockedCameraController>();


        FrameBufferSpec spec = {1280, 720};
        frameBuffer = FrameBuffer::create(spec);


        dockSpace.addWindow([&]() {
            ImGui::Begin("Statistics");
            ImGui::Text("Renderer2D Stats:");
            ImGui::Text("FrameTime: %.2fms", Renderer2D::getStats().lastFrameTime);
            ImGui::Text("FPS: %.1f", 1000.0f / Renderer2D::getStats().lastFrameTime);
            ImGui::Text("Draw Calls: %d", Renderer2D::getStats().draws);
            ImGui::Text("Quads: %d", Renderer2D::getStats().quadCount);
            ImGui::Text("Vertices: %d", Renderer2D::getStats().getTotalVertexCount());
            ImGui::Text("Indices: %d", Renderer2D::getStats().getTotalIndexCount());
            ImGui::End();
        });

        dockSpace.addWindow([&]() {
            glm::vec4& colour =
                playerEntity.getComponent<SpriteRendererComponent>().Colour;
            std::string name = playerEntity.getComponent<TagComponent>().Tag;

            ImGui::Begin("Settings");
            ImGui::Text("%s Colour: {%.0f, %.0f, %.0f, %.0f}", name.c_str(),
                        colour.x * 255.0f, colour.y * 255.0f, colour.z * 255.0f,
                        colour.w * 255.0f);

            ImGui::ColorEdit4("", glm::value_ptr(colour));

            ImGui::Separator();

            ImGui::Text("Camera Settings");
            ImGui::DragFloat2(
                "Position",
                glm::value_ptr(
                    cameraEntity.getComponent<TransformComponent>().Transform[3]),
                0.1f);

            if (ImGui::Checkbox("Camera A", &CameraA)) {
                cameraEntity.getComponent<CameraComponent>().Primary = CameraA;
                lockedCameraEntity.getComponent<CameraComponent>().Primary = !CameraA;
            }

            auto& camera = lockedCameraEntity.getComponent<CameraComponent>().Camera;
            float orthoSize = camera.getOrthographicSize();
            if (ImGui::DragFloat("LockedCamera ortho size", &orthoSize)) {
                camera.setOrthographicSize(orthoSize);
            }

            ImGui::End();
        });

        dockSpace.addWindow([&]() {
            uint32_t texID = frameBuffer->getColorAttachmentID();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
            ImGui::Begin("Viewport");

            viewportFocused = ImGui::IsWindowFocused();
            viewportHovered = ImGui::IsWindowHovered();
            Application::getImGuiLayer()->shouldAllowEvents(viewportFocused &&
                                                            viewportHovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            ImVec2 mousePos = ImGui::GetMousePos();
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 rootViewportPos = ImGui::GetMainViewport()->Pos;

            ImVec2 vpPos =
                ImVec2(windowPos.x - rootViewportPos.x, windowPos.y - rootViewportPos.y);
            ImVec2 mPosInVp = ImVec2(mousePos.x - windowPos.x, mousePos.y - windowPos.y);

            viewportSize = glm::vec2{viewportPanelSize.x, viewportPanelSize.y};
            mousePosInViewport = glm::vec2{mPosInVp.x, mPosInVp.y};
            viewportPos = glm::vec2{vpPos.x, vpPos.y};

            ImGui::Image((void*)texID, ImVec2{viewportSize.x, viewportSize.y}, {0, 1},
                         {1, 0});
            ImGui::End();
            ImGui::PopStyleVar();
        });
    }

    void CrankEditor::onDetach() {}

    void CrankEditor::onUpdate(TimeStep deltaTime) {
        if (viewportFocused) cameraController.onUpdate(deltaTime);

        if (viewportSize.x > 0.0f && viewportSize.y > 0.0f) {
            frameBuffer->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
            cameraController.onResize(viewportSize.x, viewportSize.y);

            activeScene->onResize(viewportSize.x, viewportSize.y);
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

    void CrankEditor::onEvent(Event& event) { cameraController.onEvent(event); }
}  // namespace AnEngine
