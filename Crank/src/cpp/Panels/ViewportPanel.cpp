#include <glm/gtc/type_ptr.hpp>

#include "Panels/ViewportPanel.hpp"

#include "ImGuizmo.h"

#include "Application.hpp"
#include "Core/Input.hpp"
#include "Core/KeyCodes.hpp"
#include "Maths/Maths.hpp"
#include "Panels/Panel.hpp"
#include "Panels/ScenesPanel.hpp"


namespace AnEngine::Crank {
    ViewportPanel::ViewportPanel(std::string name, const Ref<FrameBuffer>& fbuf,
                                 Ref<DockSpace>& dspace, Ref<ScenesPanel>& scenePanel)
        : name(name),
          frameBuffer(fbuf),
          dockSpace(dspace),
          sceneHierarchy(scenePanel),
          gizmoType(-1) {}

    ImGuiWindowFlags ViewportPanel::beforeRender() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        return ImGuiWindowFlags_MenuBar;
    }

    void ViewportPanel::render() {
        dockSpace->updateViewportInfo();
        Application::getImGuiLayer()->shouldAllowEvents(!dockSpace->isViewportFocused() ||
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
            ImGui::EndMenuBar();
        }

        uint32_t texID = frameBuffer->getColorAttachmentID();
        ImGui::Image((void*)texID, dockSpace->getViewportSize(), {0, 1}, {1, 0});


        // Gizmos
        Entity activeEntity = sceneHierarchy->selectedEntity;

        if (!activeEntity || gizmoType < 0) return;

        ImGuizmo::SetOrthographic(false);  // TODO: orthographic
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(dockSpace->getViewportPos().x, dockSpace->getViewportPos().y,
                          dockSpace->getViewportSize().x, dockSpace->getViewportSize().y);


        // Camera
        auto cameraEntity = sceneHierarchy->currentScene->getPrimaryCamera();
        if (!cameraEntity) return;

        const auto& camera = cameraEntity.getComponent<CameraComponent>().Camera;
        const auto& camTC = cameraEntity.getComponent<TransformComponent>();
        const glm::mat4& cameraProjection = camera.getProjectionMatrix();
        glm::mat4 cameraView = glm::inverse((glm::mat4)camTC);

        // Entity
        auto& eTC = activeEntity.getComponent<TransformComponent>();
        glm::mat4 transform = (glm::mat4)eTC;

        bool shouldSnap = Input::isKeyPressed(KeyCode::LeftControl);
        float snapValue = 0.0f;

        if (gizmoType == ImGuizmo::OPERATION::TRANSLATE) {
            snapValue = translateSnap;
        } else if (gizmoType == ImGuizmo::OPERATION::ROTATE) {
            snapValue = rotateSnap;
        }

        float snapValues[3] = {snapValue, snapValue, snapValue};

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
