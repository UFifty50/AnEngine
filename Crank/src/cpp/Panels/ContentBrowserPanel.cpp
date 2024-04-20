#include "Panels/ContentBrowserPanel.hpp"

#include <imgui.h>

#include <filesystem>

#include "Core/Log.hpp"
#include "Globals.hpp"
#include "Project/ProjectSerialiser.hpp"
#include "Texture/Texture2D.hpp"


namespace fs = std::filesystem;

namespace AnEngine::Crank {
    // TODO: add projects <-- working on this
    const fs::path g_BaseAssetsDirectory = "assets";

    ContentBrowserPanel::ContentBrowserPanel(const std::string& name)
        : name(name), currentPath(g_BaseAssetsDirectory) {
        AE_PROFILE_FUNCTION()

        fileIcon = Texture2D::create("builtins/icons/FileIcon.png");
        materialIcon = Texture2D::create("builtins/icons/MaterialIcon.png");
        directoryIcon = Texture2D::create("builtins/icons/DirectoryIcon.png");
    }

    void ContentBrowserPanel::render() {
        AE_PROFILE_FUNCTION()

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("+")) {
                if (ImGui::MenuItem("New Directory")) {
                    fs::create_directory(currentPath / "New Directory");
                    // ProjectSerialiser::createDirectory(currentPath / "New Directory");
                }
                if (ImGui::MenuItem("New Material")) {
                    Material newMat;
                    std::string newMatPath =
                        (currentPath / "New Material.aematl").string();
                    ProjectSerialiser::saveResource(newMat, newMatPath);
                }
                if (ImGui::MenuItem("New Texture")) {
                    AE_CORE_INFO("Create Texture");
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Settings")) {
                ImGui::SliderFloat("Thumbnail Size", &thumbSize, 64.0f, 256.0f);
                ImGui::SliderFloat("Padding", &paddingWidth, 0.0f, 32.0f);

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }


        if (currentPath != g_BaseAssetsDirectory) {
            if (ImGui::Button("<- Back")) {
                currentPath = currentPath.parent_path();
            }
            ImGui::SameLine();
        }

        ImGui::Text(currentPath.string().c_str());


        float cellSize = thumbSize + paddingWidth;
        uint16_t numCols =
            (uint16_t)((ImGui::GetContentRegionAvail().x - 16.0f) / cellSize);
        uint16_t numFilesInDir =
            (uint16_t)std::distance(std::filesystem::directory_iterator{currentPath},
                                    std::filesystem::directory_iterator{});
        numCols = std::min(std::max(numCols, (uint16_t)1), numFilesInDir);


        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0, 0, 0, 0});

        {
            AE_PROFILE_SCOPE("Table")

            if (ImGui::BeginTable("##Content", numCols)) {
                for (int col = 0; col < numCols; col++) {
                    ImGui::TableSetupColumn(labelFromInt(col),
                                            ImGuiTableColumnFlags_WidthFixed, cellSize);
                }
                ImGui::TableNextRow();

                uint16_t column = 0;

                for (auto& dirEnt : fs::directory_iterator(currentPath)) {
                    const fs::path path = dirEnt.path();
                    const fs::path relPath = fs::relative(path, g_BaseAssetsDirectory);
                    const std::string relPathName = relPath.filename().string();

                    Ref<Texture2D> icon;
                    if (dirEnt.is_directory()) {
                        icon = directoryIcon;
                    } else if (relPath.extension() == ".aematl") {  // TODO: ASSET MANAGER
                        icon = materialIcon;
                    } else {
                        icon = fileIcon;
                    }

                    ImGui::PushID(relPathName.c_str());

                    ImGui::TableSetColumnIndex(column);
                    ImGui::ImageButton(labelFromInt(column),
                                       (ImTextureID)icon->getSampler().slot,
                                       {thumbSize, thumbSize}, {0, 1}, {1, 0});

                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                        const PayloadType type =
                            path.extension() == ".aematl"    ? PayloadType::Material
                            : path.extension() == ".aescene" ? PayloadType::Scene
                                                             : PayloadType::Texture;

                        const DropPayload* payload = new DropPayload{path, type};

                        ImGui::SetDragDropPayload("CONTENTBROWSER_ITEM", payload,
                                                  sizeof(DropPayload));

                        ImGui::Image((ImTextureID)icon->getSampler().slot,
                                     {thumbSize, thumbSize}, {0, 1}, {1, 0});

                        ImGui::EndDragDropSource();
                    }

                    if (ImGui::IsItemHovered() &&
                        ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                        if (dirEnt.is_regular_file()) {
                            selectedItem = dirEnt.path();
                            AE_CORE_INFO("Selected: {}", selectedItem.string());
                        } else if (dirEnt.is_directory()) {
                            currentPath = dirEnt;
                            AE_CORE_INFO("Entered Directory: {}", currentPath.string());
                        }
                    }

                    ImGui::TextWrapped(relPathName.c_str());

                    if (column < (numCols - 1)) {
                        column++;
                    } else {
                        column = 0;
                        ImGui::TableNextRow();
                    }


                    ImGui::PopID();
                }

                ImGui::EndTable();
            }
        }

        ImGui::PopStyleColor();
    }
}  // namespace AnEngine::Crank
