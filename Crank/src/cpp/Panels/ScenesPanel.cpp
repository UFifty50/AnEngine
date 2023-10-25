#include <glm/gtc/type_ptr.hpp>

#include "Panels/ScenesPanel.hpp"

#include <cstdio>

#include "imgui.h"

#include "Panels/PropertiesPanel.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"


namespace AnEngine::Crank {
    ScenesPanel::ScenesPanel(std::string name, const Ref<Scene>& scene) : name(name) {
        setCurrentScene(scene);
    }

    void ScenesPanel::setCurrentScene(const Ref<Scene>& scene) { currentScene = scene; }

    void ScenesPanel::render() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {200.0f, 10.0f});

        const float ItemSpacing = ImGui::GetStyle().ItemSpacing.x;

        static float addSceneButtonWidth = 100.0f;
        static float addEntityButtonWidth = 100.0f;

        if (currentScene) {
            ImGui::Text(currentScene->getName().c_str());

            float pos = addSceneButtonWidth + ItemSpacing;
            ImGui::SameLine(ImGui::GetWindowWidth() - pos);
            ImGui::Button("V");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Scene Settings");
            addSceneButtonWidth = ImGui::GetItemRectSize().x;

            ImGui::Separator();

            ImGui::AlignTextToFramePadding();
            bool entityTreeOpen = ImGui::TreeNodeEx(
                "##Entities",
                ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap,
                "Entities");

            pos = addEntityButtonWidth + ItemSpacing;
            ImGui::SameLine(ImGui::GetWindowWidth() - pos);

            if (ImGui::Button("+")) entityUIOpen = true;
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add Entity");
            addEntityButtonWidth = ImGui::GetItemRectSize().x;

            if (entityUIOpen) drawAddEntityUI();

            if (entityTreeOpen) {
                for (const auto& [entityID] :
                     currentScene->entityRegistry.storage<entt::entity>().each()) {
                    Entity entity{entityID, currentScene.get()};
                    drawEntityNode(entity);
                }
                ImGui::TreePop();
            }
        } else {
            ImGui::Text("No scene selected");
        }

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            selectedEntity = {};
        }

        ImGui::PopStyleVar();
    }

    void ScenesPanel::drawEntityNode(Entity entity) {
        auto& tag = entity.getComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        flags |= (selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0;

        bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.c_str());
        if (ImGui::IsItemClicked()) selectedEntity = entity;

        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete Entity")) {
                entityDeleted = true;
            }
            ImGui::EndPopup();
        }

        // if (ImGui::IsMouseDoubleClicked()) {) Modify name
        if (opened) ImGui::TreePop();

        if (entityDeleted) {
            currentScene->destroyEntity(entity);
            if (selectedEntity == entity) selectedEntity = {};
        }
    }

    void ScenesPanel::drawAddEntityUI() {
        ImGui::Begin("Add Entity");

        char buffer[256] = "";
        if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
            tagName = std::string(buffer);
        }

        PropertiesPanel::drawVec3Controller("Position", transformPos, 0.0f);

        glm::vec3 rotation = glm::degrees(transformRot);
        PropertiesPanel::drawVec3Controller("Rotation", rotation, 0.0f);
        transformRot = glm::radians(rotation);

        PropertiesPanel::drawVec3Controller("Scale", transformScale, 1.0f);

        ImGui::Checkbox("Camera", &isCamera);
        if (isCamera) {
            ImGui::Indent(20.0f);
            auto& cam = cc.Camera;

            ImGui::Checkbox("Primary", &cc.Primary);

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

                ImGui::Checkbox("Fixed Aspect Ratio", &cc.FixedAspectRatio);
            }
            ImGui::Unindent(20.0f);
        }

        ImGui::Checkbox("Sprite", &isSprite);
        if (isSprite) {
            ImGui::Indent(20.0f);
            ImGui::ColorEdit4("Colour", glm::value_ptr(sRC.Colour));
            ImGui::Unindent(20.0f);
        }

        ImGui::Checkbox("Script", &isScripted);
        if (isScripted) {
            ImGui::Indent(20.0f);
            ImGui::InputTextMultiline("##source", script.data(), script.size(),
                                      ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16),
                                      ImGuiInputTextFlags_AllowTabInput);
            ImGui::Unindent(20.0f);
        }

        if (ImGui::Button("Create")) {
            Entity e = currentScene->createEntity(tagName);
            e.getComponent<TransformComponent>().Position = transformPos;
            e.getComponent<TransformComponent>().Rotation = transformRot;
            e.getComponent<TransformComponent>().Scale = transformScale;
            if (isCamera) e.addComponent<CameraComponent>(cc);
            if (isSprite) e.addComponent<SpriteRendererComponent>(sRC);
            entityUIOpen = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel")) entityUIOpen = false;
        ImGui::End();
    }
};  // namespace AnEngine::Crank
