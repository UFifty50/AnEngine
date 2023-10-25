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

    private:
        void drawComponents(Entity entity);

        template <typename T>
        void drawComponent(std::string name, Entity entity,
                           std::function<void(T&)> func) {
            if (!entity.hasComponent<T>()) {
                return;
            }

            auto& component = entity.getComponent<T>();
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
            if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str())) {
                func(component);
                ImGui::TreePop();
            }
        }

        void drawVec3Controller(const std::string& label, glm::vec3& toDraw,
                                float defaultValue = 0.0f);

        std::string name;
        Ref<ScenesPanel> scenesPanel;
    };
};  // namespace AnEngine::Crank

#endif
