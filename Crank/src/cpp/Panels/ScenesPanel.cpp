#include <glm/gtc/type_ptr.hpp>

#include "Panels/ScenesPanel.hpp"

#include "imgui.h"

#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"


namespace AnEngine::Crank {
    ScenesPanel::ScenesPanel(std::string name, const Ref<Scene>& scene) : name(name) {
        setCurrentScene(scene);
    }

    void ScenesPanel::setCurrentScene(const Ref<Scene>& scene) { currentScene = scene; }

    void ScenesPanel::render() {
        if (currentScene) {
            ImGui::Text(currentScene->getName().c_str());
            ImGui::Separator();
            if (ImGui::TreeNode("Entities")) {
                currentScene->entityRegistry.each([&](auto& entityID) {
                    Entity entity{entityID, currentScene.get()};
                    drawEntityNode(entity);
                });
                ImGui::TreePop();
            }
        } else {
            ImGui::Text("No scene selected");
        }

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            selectedEntity = {};
        }
    }

    void ScenesPanel::drawEntityNode(Entity entity) {
        auto& tag = entity.getComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= (selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0;

        bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.c_str());
        if (ImGui::IsItemClicked()) selectedEntity = entity;
        if (opened) ImGui::TreePop();
    }
};  // namespace AnEngine::Crank
