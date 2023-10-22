#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Panels/PropertiesPanel.hpp"

#include "imgui.h"

#include "Panels/ScenesPanel.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"


#define HASH(type) (void*)typeid(type).hash_code()


namespace AnEngine::Crank {
    PropertiesPanel::PropertiesPanel(std::string name, Ref<ScenesPanel> scenesPanel)
        : name(name), scenesPanel(scenesPanel) {
        AE_CORE_INFO("");
    }

    void PropertiesPanel::render() {
        Entity selectedEntity = scenesPanel->getSelectedEntity();

        if (selectedEntity) drawComponents(selectedEntity);
    }

    void PropertiesPanel::drawComponents(Entity entity) {
        if (entity.hasComponent<TagComponent>()) {
            auto& tag = entity.getComponent<TagComponent>().Tag;
            char buffer[256] = {0};
            if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
                tag = std::string(buffer);
            }
        }

        drawComponent<TransformComponent>("Transform", entity, [](auto& component) {
            ImGui::DragFloat3("Position", glm::value_ptr(component.Transform[3]), 0.1f);
        });

        drawComponent<CameraComponent>("Camera", entity, [](auto& component) {
            auto& cam = component.Camera;

            ImGui::Checkbox("Primary", &component.Primary);

            const char* projectionTypes[] = {"perspective", "orthographic"};
            const char* currentProjectionType = projectionTypes[(int)cam.getType()];

            if (ImGui::BeginCombo("Projection Type",
                                  projectionTypes[(int)cam.getType()])) {
                bool isPerspective = currentProjectionType == projectionTypes[0];
                bool isOrthographic = !isPerspective;

                if (ImGui::Selectable(projectionTypes[0], isPerspective)) {
                    currentProjectionType = projectionTypes[0];
                    cam.setType(ProjectionType::Perspective);
                }

                if (ImGui::Selectable(projectionTypes[1], isOrthographic)) {
                    currentProjectionType = projectionTypes[1];
                    cam.setType(ProjectionType::Orthographic);
                }

                ImGui::SetItemDefaultFocus();

                ImGui::EndCombo();
            }

            if (cam.getType() == ProjectionType::Perspective) {
                float fov = glm::degrees(cam.getPerspectiveFOV());
                float near = cam.getPerspectiveNear();
                float far = cam.getPerspectiveFar();

                if (ImGui::DragFloat("Field Of View", &fov, 1.0f, 10.0f, 150.0f))
                    cam.setPerspectiveFOV(glm::radians(fov));

                if (ImGui::DragFloat("Near", &near, 0.5f, -far, far - 0.01f))
                    cam.setPerspectiveNear(near);

                if (ImGui::DragFloat("Far", &far, 0.5f, near + 0.01f, 1000.0f))
                    cam.setPerspectiveFar(far);
            }

            if (cam.getType() == ProjectionType::Orthographic) {
                float size = cam.getOrthographicSize();
                float near = cam.getOrthographicNear();
                float far = cam.getOrthographicFar();

                if (ImGui::DragFloat("Size", &size, 1.0f, 1.0f, 200.0f))
                    cam.setOrthographicSize(size);

                if (ImGui::DragFloat("Near", &near, 0.5f, -far, far - 0.01f))
                    cam.setOrthographicNear(near);

                if (ImGui::DragFloat("Far", &far, 0.5f, near + 0.01f, 1000.0f))
                    cam.setOrthographicFar(far);

                ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
            }
        });

        drawComponent<SpriteRendererComponent>(
            "Sprite Renderer", entity, [](auto& component) {
                ImGui::ColorEdit4("Colour", glm::value_ptr(component.Colour));
            });
    }
}  // namespace AnEngine::Crank
