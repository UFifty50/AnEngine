#include <glm/gtc/type_ptr.hpp>

#include "Panels/ScenesPanel.hpp"

#include <algorithm>
#include <cstdio>
#include <ranges>

#include "imgui.h"

#include "Globals.hpp"
#include "Panels/PropertiesPanel.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"


namespace AnEngine::Crank {
    ScenesPanel::ScenesPanel(const std::string& name) : name(name) {
        AE_PROFILE_FUNCTION()

        updateCurrentSceneFromActive();
    }

    void ScenesPanel::updateCurrentSceneFromActive() {
        AE_PROFILE_FUNCTION()

        currentScene = g_ActiveScene;
        selectedEntity = {};  // TODO: set to uuid from file
    }

    void ScenesPanel::render() {
        AE_PROFILE_FUNCTION()

        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {200.0f, 10.0f});

        const float ItemSpacing = ImGui::GetStyle().ItemSpacing.x;

        static float sceneSettingsButtonWidth = 100.0f;
        static float addEntityButtonWidth = 100.0f;
        // static bool editingName = false;

        if (currentScene) {
            /*if (editingName) {
                std::string ogName = currentScene->name;

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {4.0f, 4.0f});
                ImGui::InputTextEx("##SceneName", currentScene->name.c_str(),
                                   currentScene->name.data(), 256, {0.0f, 0.0f}, 0);
                ImGui::PopStyleVar();

                if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) ||
                    ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_KeyPadEnter))) {
                    editingName = false;
                }

                ImGui::SameLine();
                bool shouldClose = ImGui::Button("X");
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Close");
                if (shouldClose) {
                    currentScene->name = ogName;
                    editingName = false;
                }
            } else {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {4.0f, 4.0f});
                ImGui::PushStyleColor(ImGuiCol_Text, {0.8f, 0.8f, 0.8f, 1.0f});
                ImGui::Button(currentScene->name.c_str());
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();

                if (ImGui::IsItemClicked()) editingName = true;
            }*/

            ImGui::Text(currentScene->getName().c_str());

            float pos = sceneSettingsButtonWidth + ItemSpacing;
            ImGui::SameLine(ImGui::GetWindowWidth() - pos);
            ImGui::Button("V");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Scene Settings");
            sceneSettingsButtonWidth = ImGui::GetItemRectSize().x;

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

            //   if (entityUIOpen) drawAddEntityUI();
            if (entityUIOpen) {
                Entity e = currentScene->createEntity(tagName);
                entityUIOpen = false;
                std::ranges::fill(std::ranges::views::values(isEntityBeingEdited), false);
            }

            if (entityTreeOpen) {
                for (const auto& [entityID] :
                     currentScene->getRegistry().storage<entt::entity>().each()) {
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
        AE_PROFILE_FUNCTION()

        auto& tag = entity.getComponent<TagComponent>().Tag;
        tag.resize(255);

        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        flags |= (selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0;

        bool opened = false;
        if (!isEntityBeingEdited[(uint32_t)entity])
            opened = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.c_str());
        else {
            ImGui::InputTextWithHint(("##" + std::to_string((uint32_t)entity)).c_str(),
                                     "Enter a new tag", tag.data(), tag.size() + 1);
            if (ImGui::IsKeyPressed(ImGuiKey_Enter) ||
                (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)))
                isEntityBeingEdited[(uint32_t)entity] = false;
        }

        if (!isEntityBeingEdited[(uint32_t)entity] && ImGui::IsItemHovered() &&
            ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            isEntityBeingEdited[(uint32_t)entity] = true;
        else if (!isEntityBeingEdited[(uint32_t)entity] && ImGui::IsItemClicked())
            selectedEntity = entity;

        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete Entity")) {
                entityDeleted = true;
            }
            ImGui::EndPopup();
        }

        if (opened) ImGui::TreePop();

        if (entityDeleted) {
            currentScene->destroyEntity(entity);
            if (selectedEntity == entity) selectedEntity = {};
        }
    }

    /*
    void ScenesPanel::drawAddEntityUI() {
        ImGui::Begin("Add Entity", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);

        char buffer[256] = "";
        if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
            tagName = std::string(buffer);
        }

        // TODO move to Utils namespace
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

            if (ImGui::BeginCombo("Projection Type",
                                  cam.getProjectionType().toString().c_str())) {
                bool isPerspective =
                    cam.getProjectionType() == ProjectionType::Perspective;

                if (ImGui::Selectable("Perspective", isPerspective)) {
                    cam.changeProjectionType(ProjectionType::Perspective);
                }

                if (ImGui::Selectable("Orthographic", !isPerspective)) {
                    cam.changeProjectionType(ProjectionType::Orthographic);
                }

                ImGui::SetItemDefaultFocus();

                ImGui::EndCombo();
            }

            if (cam.getProjectionType() == ProjectionType::Perspective) {
                float fov = glm::degrees(cam.getFOV());
                float near = cam.getNearPlane();
                float far = cam.getFarPlane();

                if (ImGui::DragFloat("Field Of View", &fov, 1.0f, 10.0f, 150.0f))
                    cam.updateSpec(CameraSpec3D::Feild::FOVorSize, glm::radians(fov));

                if (ImGui::DragFloat("Near", &near, 0.5f, -far, far - 0.01f))
                    cam.updateSpec(CameraSpec3D::Feild::NearPlane, near);

                if (ImGui::DragFloat("Far", &far, 0.5f, near + 0.01f, 1000.0f))
                    cam.updateSpec(CameraSpec3D::Feild::FarPlane, far);
            }

            if (cam.getProjectionType() == ProjectionType::Orthographic) {
                float size = cam.getOrthoSize();
                float near = cam.getNearPlane();
                float far = cam.getFarPlane();

                if (ImGui::DragFloat("Size", &size, 1.0f, 1.0f, 200.0f))
                    cam.updateSpec(CameraSpec3D::Feild::FOVorSize, size);

                if (ImGui::DragFloat("Near", &near, 0.5f, -far, far - 0.01f))
                    cam.updateSpec(CameraSpec3D::Feild::NearPlane, near);

                if (ImGui::DragFloat("Far", &far, 0.5f, near + 0.01f, 1000.0f))
                    cam.updateSpec(CameraSpec3D::Feild::FarPlane, far);

                ImGui::Checkbox("Fixed Aspect Ratio", &cc.FixedAspectRatio);

                if (cc.FixedAspectRatio) {
                    float aspectRatio = cam.getAspectRatio();
                    if (ImGui::DragFloat("Aspect Ratio", &aspectRatio, 0.05f, 0.1f,
                                         10.0f))
                        cam.updateSpec(CameraSpec3D::Feild::AspectRatio, aspectRatio);
                }
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
    */

};  // namespace AnEngine::Crank
