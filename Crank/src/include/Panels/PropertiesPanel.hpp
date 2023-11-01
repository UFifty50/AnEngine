#ifndef PROPERTIESPANEL_HPP
#define PROPERTIESPANEL_HPP

#include <entt/entt.hpp>

#include <functional>

#include "imgui_internal.h"

#include "Panels/Panel.hpp"
#include "Panels/ScenesPanel.hpp"
#include "Scene/Entity.hpp"


namespace AnEngine::Crank {
    class PropertiesPanel : public Panel {
    public:
        PropertiesPanel() = default;
        PropertiesPanel(std::string name, Ref<ScenesPanel> scenesPanel);

        virtual ImGuiWindowFlags beforeRender() override {
            //    ImGui::SetNextWindowSizeConstraints({370.0f, -1}, {INFINITY, INFINITY});
            return 0;
        }

        virtual void render() override;
        virtual void afterRender() override {}

        virtual void onClose() override {}

        virtual std::string getName() override { return name; }

        static void drawVec3Controller(const std::string& label, glm::vec3& toDraw,
                                       float defaultValue = 0.0f);

    private:
        void drawComponents(Entity entity);

        template <typename T>
        void drawComponent(const std::string& name, Entity entity, bool removable,
                           std::function<void(T&)> func, ImGuiTreeNodeFlags flags = 0) {
            if (!entity.hasComponent<T>()) {
                return;
            }

            ImVec2 availableRegion = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {4, 4});

            float lineHeight =
                GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

            // ImGui::Separator();

            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, "%s",
                                          name.c_str());
            if (removable) ImGui::SameLine(availableRegion.x - lineHeight * 1.5f);

            bool buttonClicked =
                removable && ImGui::Button("...", {lineHeight * 1.5f, lineHeight});
            if (removable && ImGui::IsItemHovered())
                ImGui::SetTooltip("Component Settings");
            if (removable && buttonClicked) {
                ImGui::OpenPopup("##ComponentSettings");
            }

            ImGui::PopStyleVar();

            bool removeComponent = false;
            if (ImGui::BeginPopup("##ComponentSettings")) {
                if (ImGui::MenuItem("Remove Component")) removeComponent = true;
                ImGui::EndPopup();
            }

            auto& component = entity.getComponent<T>();
            if (open) {
                func(component);
                ImGui::TreePop();
            }

            if (removable && removeComponent) entity.removeComponent<T>();
        }

        std::string name;
        Ref<ScenesPanel> scenesPanel;
    };
};  // namespace AnEngine::Crank

#endif
