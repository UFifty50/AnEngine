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


        dockSpace.addWindow(DockedWindow{
            "Statistics", [&]() {
                ImGui::Text("Renderer2D Stats:");
                ImGui::Text("FrameTime: %.2fms", Renderer2D::getStats().lastFrameTime);
                ImGui::Text("FPS: %.1f", 1000.0f / Renderer2D::getStats().lastFrameTime);
                ImGui::Text("Draw Calls: %d", Renderer2D::getStats().draws);
                ImGui::Text("Quads: %d", Renderer2D::getStats().quadCount);
                ImGui::Text("Vertices: %d", Renderer2D::getStats().getTotalVertexCount());
                ImGui::Text("Indices: %d", Renderer2D::getStats().getTotalIndexCount());
            }});

        dockSpace.addWindow(DockedWindow{
            "Settings", [&]() {
                glm::vec4& colour =
                    playerEntity.getComponent<SpriteRendererComponent>().Colour;
                std::string name = playerEntity.getComponent<TagComponent>().Tag;

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
            }});

        dockSpace.addWindow(DockedWindow{
            "Viewport",
            [&]() {
                uint32_t texID = frameBuffer->getColorAttachmentID();

                ImGui::Image((void*)texID, dockSpace.getViewportSize(), {0, 1}, {1, 0});

                dockSpace.updateViewportInfo();
            },
            {{ImGuiStyleVar_WindowPadding, ImVec2{0, 0}}}});
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
