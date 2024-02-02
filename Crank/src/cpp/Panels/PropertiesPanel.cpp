#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Panels/PropertiesPanel.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include "Globals.hpp"
#include "Panels/ContentBrowserPanel.hpp"
#include "Panels/ScenesPanel.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"
#include "Scene/ScriptableEntity.hpp"


#define HASH(type) (void*)typeid(type).hash_code()


namespace AnEngine::Crank {
    PropertiesPanel::PropertiesPanel(const std::string& name) : name(name) {}

    void PropertiesPanel::render() {
        AE_PROFILE_FUNCTION()

        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {370.0f, 10.0f});

        Entity selectedEntity = gPanel_SceneHierarchy->getSelectedEntity();

        if (selectedEntity) {
            if (selectedEntity.hasComponent<TagComponent>()) {
                auto& tag = selectedEntity.getComponent<TagComponent>().Tag;
                char buffer[256];
                std::strncpy(buffer, tag.c_str(), sizeof(buffer));
                if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
                    tag = std::string(buffer);
                }
            }

            ImGui::SameLine();
            ImGui::PushItemWidth(-1);

            if (ImGui::Button("Add Component")) ImGui::OpenPopup("AddComponent");

            ImGui::PopItemWidth();

            drawComponents(selectedEntity);
        }


        if (ImGui::BeginPopup("AddComponent")) {
            if (ImGui::MenuItem("Camera")) {
                if (!selectedEntity.hasComponent<CameraComponent>()) {
                    selectedEntity.addComponent<CameraComponent>();
                } else {
                    AE_CORE_WARN("This entity already has a camera component!");
                }

                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Sprite Renderer")) {
                if (!selectedEntity.hasComponent<SpriteRendererComponent>()) {
                    selectedEntity.addComponent<SpriteRendererComponent>();
                } else {
                    AE_CORE_WARN("This entity already has a sprite component!");
                }

                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Script")) {
                if (!selectedEntity.hasComponent<NativeScriptComponent>()) {
                    char name[256] = "Temporary Empty Script";
                    class temp : public ScriptableEntity {};
                    selectedEntity.addComponent<NativeScriptComponent>(name).bind<temp>();
                } else {
                    AE_CORE_WARN("This entity already has a script component!");
                }

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::PopStyleVar();
    }

    void PropertiesPanel::drawVec3Controller(const std::string& label, glm::vec3& values,
                                             float resetValue) {
        AE_PROFILE_FUNCTION()

        ImGuiIO& io = ImGui::GetIO();
        auto& boldFont = io.Fonts->Fonts[1];

        ImGui::PushID(label.c_str());

        if (ImGui::BeginTable("##Vec3Table", 7,
                              ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX |
                                  ImGuiTableFlags_SizingFixedFit |
                                  ImGuiTableFlags_NoHostExtendX)) {
            ImGui::TableSetupColumn(label.c_str(), ImGuiTableColumnFlags_WidthFixed,
                                    75.0f);
            ImGui::TableSetupColumn("##Xb", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("##Yb", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("##Zb", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##Z", ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableNextRow();

            for (int i = 0; i < 7; i++) {
                ImGui::TableSetColumnIndex(i);
                ImGui::PushItemWidth(-FLT_MIN);
            }

            float lineHeight =
                GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImVec2 buttonSize = {lineHeight, lineHeight};

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", label.c_str());

            ImGui::PushStyleColor(ImGuiCol_Button, {0.8f, 0.1f, 0.15f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.9f, 0.2f, 0.2f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.8f, 0.1f, 0.15f, 1.0f});
            ImGui::PushFont(boldFont);

            ImGui::TableSetColumnIndex(1);
            if (ImGui::Button("X", buttonSize)) values.x = resetValue;

            ImGui::PopFont();
            ImGui::PopStyleColor(3);

            ImGui::TableSetColumnIndex(2);
            ImGui::DragFloat("##X", &values.x, 0.05f, 0.0f, 0.0f, "%.2f");

            ImGui::PushStyleColor(ImGuiCol_Button, {0.2f, 0.7f, 0.2f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.3f, 0.8f, 0.3f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.2f, 0.7f, 0.2f, 1.0f});
            ImGui::PushFont(boldFont);

            ImGui::TableSetColumnIndex(3);
            if (ImGui::Button("Y", buttonSize)) values.y = resetValue;

            ImGui::PopFont();
            ImGui::PopStyleColor(3);

            ImGui::TableSetColumnIndex(4);
            ImGui::DragFloat("##Y", &values.y, 0.05f, 0.0f, 0.0f, "%.2f");

            ImGui::PushStyleColor(ImGuiCol_Button, {0.1f, 0.25f, 0.8f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.2f, 0.35f, 0.9f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.1f, 0.25f, 0.8f, 1.0f});
            ImGui::PushFont(boldFont);

            ImGui::TableSetColumnIndex(5);
            if (ImGui::Button("Z", buttonSize)) values.z = resetValue;

            ImGui::PopFont();
            ImGui::PopStyleColor(3);

            ImGui::TableSetColumnIndex(6);
            ImGui::DragFloat("##Z", &values.z, 0.05f, 0.0f, 0.0f, "%.2f");

            // End the table
            ImGui::EndTable();
        }


        ImGui::PopID();
    }

    void PropertiesPanel::drawComponents(Entity entity) {
        AE_PROFILE_FUNCTION()

        const ImGuiTreeNodeFlags treeNodeFlags =
            ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap |
            ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed |
            ImGuiTreeNodeFlags_FramePadding;

        drawComponent<TransformComponent>(
            "Transform", entity, false,
            [&](auto& component) {
                drawVec3Controller("Position", component.Position, 0.0f);

                glm::vec3 rotation = glm::degrees(component.Rotation);
                drawVec3Controller("Rotation", rotation, 0.0f);
                component.Rotation = glm::radians(rotation);

                drawVec3Controller("Scale", component.Scale, 1.0f);
            },
            treeNodeFlags);

        drawComponent<CameraComponent>(
            "Camera", entity, true,
            [](auto& component) {
                auto& cam = component.Camera;

                ImGui::Checkbox("Primary", &component.Primary);

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

                    ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);

                    if (component.FixedAspectRatio) {
                        float aspectRatio = cam.getAspectRatio();
                        if (ImGui::DragFloat("Aspect Ratio", &aspectRatio, 0.05f, 0.1f,
                                             10.0f))
                            cam.updateSpec(CameraSpec3D::Feild::AspectRatio, aspectRatio);
                    }
                }
            },
            treeNodeFlags);

        drawComponent<SpriteRendererComponent>(
            "Sprite Renderer", entity, true,
            [](auto& component) {
                ImGui::ColorEdit4("Colour",
                                  glm::value_ptr(component.SpriteMaterial.colour));

                if (ImGui::BeginTable("##MaterialTable", 2)) {
                    ImGui::TableSetupColumn("##MaterialView",
                                            ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("##MaterialInfo");

                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    if (auto tex = component.SpriteMaterial.getTexture())
                        ImGui::ImageButton("##E", (ImTextureID)(*tex)->getSampler().slot,
                                           {50.0f, 50.0f}, {0, 1}, {1, 0});
                    else
                        ImGui::Button("##Texture", {50.0f, 50.0f});

                    if (ImGui::BeginDragDropTarget()) {
                        if (const ImGuiPayload* payload =
                                ImGui::AcceptDragDropPayload("CONTENTBROWSER_ITEM")) {
                            const wchar_t* path = (const wchar_t*)payload->Data;

                            component.SpriteMaterial.texture =
                                Texture2D::create(g_BaseAssetsDirectory / path);
                        }

                        ImGui::EndDragDropTarget();
                    }

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("Placeholder Name");
                    ImGui::SameLine();
                    if (ImGui::Button("X")) component.SpriteMaterial.texture = nullptr;

                    if (ImGui::TreeNodeEx("Shader")) ImGui::TreePop();
                    ImGui::SameLine();
                    ImGui::Button("Edit");

                    ImGui::EndTable();
                }
            },
            treeNodeFlags);


        drawComponent<NativeScriptComponent>(
            "Script", entity, true,
            [&](auto& component) { ImGui::Text("%s", component.Name.c_str()); },
            treeNodeFlags);
    }
}  // namespace AnEngine::Crank


/// [image] [name] [close]
///         [shader] [edit]
