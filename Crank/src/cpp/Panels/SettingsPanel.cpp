#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Panels/SettingsPanel.hpp"

#include "imgui.h"

#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"


namespace AnEngine::Crank {
    SettingsPanel::SettingsPanel(std::string name, Entity& p, Entity& c, Entity& lC,
                                 bool& cA)
        : name(name), player(p), camera(c), lockedCamera(lC), CameraA(cA) {}

    void SettingsPanel::begin() { ImGui::Begin(name.c_str()); }

    void SettingsPanel::render() {
        glm::vec4& colour = player.getComponent<SpriteRendererComponent>().Colour;
        std::string name = player.getComponent<TagComponent>().Tag;

        ImGui::Text("%s Colour: {%.0f, %.0f, %.0f, %.0f}", name.c_str(),
                    colour.x * 255.0f, colour.y * 255.0f, colour.z * 255.0f,
                    colour.w * 255.0f);

        ImGui::ColorEdit4("", glm::value_ptr(colour));

        ImGui::Separator();

        ImGui::Text("Camera Settings");
        ImGui::DragFloat2(
            "Position",
            glm::value_ptr(camera.getComponent<TransformComponent>().Transform[3]), 0.1f);

        if (ImGui::Checkbox("Camera A", &CameraA)) {
            camera.getComponent<CameraComponent>().Primary = CameraA;
            lockedCamera.getComponent<CameraComponent>().Primary = !CameraA;
        }

        auto& camera = lockedCamera.getComponent<CameraComponent>().Camera;
        float orthoSize = camera.getOrthographicSize();
        if (ImGui::DragFloat("LockedCamera ortho size", &orthoSize)) {
            camera.setOrthographicSize(orthoSize);
        }
    }

    void SettingsPanel::end() { ImGui::End(); }
}  // namespace AnEngine::Crank
