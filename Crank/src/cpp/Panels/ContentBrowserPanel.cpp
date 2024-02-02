#include "Panels/ContentBrowserPanel.hpp"

#include <imgui.h>

#include <filesystem>

#include "Core/Log.hpp"
#include "Globals.hpp"
#include "Texture/Texture2D.hpp"


namespace fs = std::filesystem;

namespace AnEngine::Crank {
    const fs::path g_BaseAssetsDirectory = "assets";  // TODO: add projects

    ContentBrowserPanel::ContentBrowserPanel(const std::string& name)
        : name(name), currentPath(g_BaseAssetsDirectory) {
        AE_PROFILE_FUNCTION()

        directoryIcon = Texture2D::create("builtins/icons/DirectoryIcon.png");
        fileIcon = Texture2D::create("builtins/icons/FileIcon.png");
    }

    void ContentBrowserPanel::render() {
        AE_PROFILE_FUNCTION()

        if (ImGui::BeginMenuBar()) {
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
                    const Ref<Texture2D> icon =
                        dirEnt.is_directory() ? directoryIcon : fileIcon;

                    ImGui::PushID(relPathName.c_str());

                    ImGui::TableSetColumnIndex(column);
                    ImGui::ImageButton(labelFromInt(column),
                                       (ImTextureID)icon->getSampler().slot,
                                       {thumbSize, thumbSize}, {0, 1}, {1, 0});

                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                        const wchar_t* itemPath = relPath.c_str();
                        ImGui::SetDragDropPayload(
                            "CONTENTBROWSER_ITEM", itemPath,
                            (wcslen(itemPath) + 1) * sizeof(wchar_t));

                        ImGui::Image((ImTextureID)icon->getSampler().slot,
                                     {thumbSize, thumbSize}, {0, 1}, {1, 0});

                        ImGui::EndDragDropSource();
                    }

                    if (ImGui::IsItemHovered() &&
                        ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                        currentPath = currentPath / path.filename();
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
