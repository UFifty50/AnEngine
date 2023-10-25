#ifndef PROPERTIESPANEL_HPP
#define PROPERTIESPANEL_HPP

#include <entt/entt.hpp>

#include <functional>

#include "Panels/Panel.hpp"
#include "Panels/ScenesPanel.hpp"
#include "Scene/Entity.hpp"


namespace AnEngine::Crank {
    class PropertiesPanel : public Panel {
    public:
        PropertiesPanel(std::string name, Ref<ScenesPanel> scenesPanel);

        virtual void beforeRender() override {}
        virtual void render() override;
        virtual void afterRender() override {}

        virtual std::string getName() override { return name; }

        static void drawVec3Controller(const std::string& label, glm::vec3& toDraw,
                                       float defaultValue = 0.0f);

    private:
        void drawComponents(Entity entity);

        template <typename T>
        void drawComponent(std::string name, Entity entity, bool removable,
                           std::function<void(T&)> func, ImGuiTreeNodeFlags flags = 0) {
            if (!entity.hasComponent<T>()) {
                return;
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {4, 4});
            bool open =
                ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());
            if (removable) ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);

            bool buttonClicked = removable && ImGui::Button("...", {20, 20});
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
