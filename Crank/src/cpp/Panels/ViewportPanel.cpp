#include <glm/gtc/type_ptr.hpp>

#include "Panels/ViewportPanel.hpp"

#include "ImGuizmo.h"

#include "Application.hpp"
#include "Core/Input.hpp"
#include "Core/KeyCodes.hpp"
#include "Maths/Maths.hpp"
#include "Menus/Filemenu.hpp"
#include "Panels/Panel.hpp"
#include "Panels/ScenesPanel.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
#include "Renderer/FrameBuffer.hpp"


namespace AnEngine::Crank {
    extern const fs::path baseAssetsDirectory;

    ViewportPanel::ViewportPanel(std::string name, const Ref<FrameBuffer>& fbuf,
                                 Ref<EditorCamera>& editorCamera, Ref<DockSpace>& dspace,
                                 Ref<ScenesPanel>& scenePanel, Ref<Scene>& activeScene)
        : name(name),
          frameBuffer(fbuf),
          editorCamera(editorCamera),
          dockSpace(dspace),
          sceneHierarchy(scenePanel),
          activeScene(activeScene),
          gizmoType(ImGuizmo::OPERATION::TRANSLATE),
          translateSnap(0.0f),
          rotateSnap(0.0f) {}

    ImGuiWindowFlags ViewportPanel::beforeRender() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;

        ImGui::SetNextWindowClass(&window_class);
        return ImGuiWindowFlags_MenuBar;
    }

    void ViewportPanel::render() {
        static uint32_t fbID = 0;
        dockSpace->updateViewportInfo(true, true);

        Application::getImGuiLayer()->shouldAllowEvents(!dockSpace->isViewportFocused() &&
                                                        !dockSpace->isViewportHovered());
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("translation type")) {
                if (ImGui::MenuItem("Position"))
                    gizmoType = ImGuizmo::OPERATION::TRANSLATE;
                if (ImGui::MenuItem("Rotation")) gizmoType = ImGuizmo::OPERATION::ROTATE;
                if (ImGui::MenuItem("Scale")) gizmoType = ImGuizmo::OPERATION::SCALE;
                if (ImGui::MenuItem("All")) gizmoType = ImGuizmo::OPERATION::UNIVERSAL;
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Snapping")) {
                if (gizmoType == ImGuizmo::OPERATION::TRANSLATE) {
                    if (ImGui::MenuItem("None")) translateSnap = 0.0f;
                    if (ImGui::MenuItem("Snap to 0.1 meter grid")) translateSnap = 0.1f;
                    if (ImGui::MenuItem("Snap to 0.5 meter grid")) translateSnap = 0.5f;
                    if (ImGui::MenuItem("Snap to 1 meter grid")) translateSnap = 1.0f;
                } else if (gizmoType == ImGuizmo::OPERATION::ROTATE) {
                    if (ImGui::MenuItem("None")) rotateSnap = 0.0f;
                    if (ImGui::MenuItem("Snap to 10 degrees")) rotateSnap = 10.0f;
                    if (ImGui::MenuItem("Snap to 45 degrees")) rotateSnap = 45.0f;
                    if (ImGui::MenuItem("Snap to 90 degrees")) rotateSnap = 90.0f;
                } else {
                    ImGui::Text("Snapping not available for this mode.");
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Editor Camera Type")) {
                if (ImGui::MenuItem("Perspective")) {
                    editorCamera->changeProjectionType(ProjectionType::Perspective);
                }
                if (ImGui::MenuItem("Orthographic")) {
                    editorCamera->changeProjectionType(ProjectionType::Orthographic);
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Framebuffer")) {
                if (ImGui::MenuItem("No. 1")) fbID = 0;
                if (ImGui::MenuItem("No. 2")) fbID = 1;
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        uint32_t texID = frameBuffer->getColorAttachmentID(fbID);
        ImGui::Image((void*)texID, dockSpace->getViewportSize(), {0, 1}, {1, 0});


        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload =
                    ImGui::AcceptDragDropPayload("CONTENTBROWSER_ITEM")) {
                const wchar_t* path = (const wchar_t*)payload->Data;

                if (auto newScene = FileMenu::OpenScene(
                        fs::path(baseAssetsDirectory) / path, sceneHierarchy, dockSpace))
                    activeScene = *newScene;
                else {
                    AE_CORE_ERROR("Couldn't load Scene {}", (char*)path);
                    return;
                }
            }
        }


        // Gizmos
        Entity activeEntity = sceneHierarchy->selectedEntity;

        if (!activeEntity || gizmoType < 0) return;

        // ImGuizmo::SetOrthographic(!editorCamera->isPerspective()); //TODO: orthographic
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(dockSpace->getViewportPos().x, dockSpace->getViewportPos().y,
                          dockSpace->getViewportSize().x, dockSpace->getViewportSize().y);


        // Camera
        // auto cameraEntity = sceneHierarchy->currentScene->getPrimaryCamera();
        // if (!cameraEntity) return;

        // const auto& camera = cameraEntity.getComponent<CameraComponent>().Camera;
        // const auto& camTC = cameraEntity.getComponent<TransformComponent>();
        //  const glm::mat4& cameraProjection = camera.getProjectionMatrix();
        //  glm::mat4 cameraView = glm::inverse((glm::mat4)camTC);

        const glm::mat4& cameraProjection = editorCamera->getProjectionMatrix();
        glm::mat4 cameraView = editorCamera->getViewMatrix();

        // Entity
        auto& eTC = activeEntity.getComponent<TransformComponent>();
        glm::mat4 transform = (glm::mat4)eTC;

        bool shouldSnap = Input::isKeyPressed(KeyCode::LeftControl) ||
                          Input::isKeyPressed(KeyCode::RightControl);

        float snapValues[3]{};
        if (gizmoType == ImGuizmo::OPERATION::TRANSLATE) {
            snapValues[0] = translateSnap;
            snapValues[1] = translateSnap;
            snapValues[2] = translateSnap;
        } else if (gizmoType == ImGuizmo::OPERATION::ROTATE) {
            snapValues[0] = rotateSnap;
            snapValues[1] = rotateSnap;
            snapValues[2] = rotateSnap;
        }


        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                             (ImGuizmo::OPERATION)gizmoType, ImGuizmo::MODE::LOCAL,
                             glm::value_ptr(transform), nullptr,
                             shouldSnap ? snapValues : nullptr);

        if (ImGuizmo::IsUsing()) {
            if (auto decomposed = Maths::decomposeTranslation(transform)) {
                const auto& [translation, rotation, scale] = *decomposed;
                glm::vec3 deltaRotation =
                    rotation - eTC.Rotation;  // counter gimball lock

                eTC.Position = translation;
                eTC.Rotation += deltaRotation;
                eTC.Scale = scale;
            }
        }
    }

    void ViewportPanel::afterRender() { ImGui::PopStyleVar(); }
}  // namespace AnEngine::Crank
