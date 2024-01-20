#include "Panels/ContentBrowserPanel.hpp"

#include <imgui.h>

#include <filesystem>

#include "Core/Log.hpp"
#include "Texture/Texture2D.hpp"


namespace fs = std::filesystem;

namespace AnEngine::Crank {
    extern const fs::path baseAssetsDirectory = "assets";  // TODO: add projects

    ContentBrowserPanel::ContentBrowserPanel(std::string name)
        : name(name), currentPath(baseAssetsDirectory) {
        directoryIcon = Texture2D::create("builtins/icons/DirectoryIcon.png");
        fileIcon = Texture2D::create("builtins/icons/FileIcon.png");
    }

    void ContentBrowserPanel::render() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Settings")) {
                ImGui::SliderFloat("Thumbnail Size", &thumbSize, 64.0f, 256.0f);
                ImGui::SliderFloat("Padding", &paddingWidth, 0.0f, 32.0f);

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }


        if (currentPath != baseAssetsDirectory) {
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

        if (ImGui::BeginTable("##Content", numCols)) {
            for (int i = 0; i < numCols; i++) {
                ImGui::TableSetupColumn(("##" + std::to_string(i)).c_str(),
                                        ImGuiTableColumnFlags_WidthFixed, cellSize);
            }
            int idx = 0;

            ImGui::TableNextRow();


            for (auto& dirEnt : fs::directory_iterator(currentPath)) {
                const fs::path& path = dirEnt.path();

                fs::path relPath = fs::relative(path, baseAssetsDirectory);
                std::string relPathName = relPath.filename().string();

                ImGui::PushID(relPathName.c_str());
                Ref<Texture2D> icon = dirEnt.is_directory() ? directoryIcon : fileIcon;

                ImGui::TableSetColumnIndex(idx);
                ImGui::ImageButton(("##" + std::to_string(idx)).c_str(),
                                   (ImTextureID)icon->getSampler().slot,
                                   {thumbSize, thumbSize}, {0, 1}, {1, 0});

                if (ImGui::BeginDragDropSource()) {
                    const wchar_t* itemPath = relPath.c_str();
                    ImGui::SetDragDropPayload("CONTENTBROWSER_ITEM", itemPath,
                                              (wcslen(itemPath) + 1) * sizeof(wchar_t));
                    ImGui::EndDragDropSource();
                }

                if (ImGui::IsItemHovered() &&
                    ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    currentPath = currentPath / path.filename();
                }

                ImGui::TextWrapped(relPathName.c_str());

                if (idx + 1 < numCols) {
                    idx++;
                } else {
                    idx = 0;
                    ImGui::TableNextRow();
                }

                ImGui::PopID();
            }

            ImGui::EndTable();
        }

        ImGui::PopStyleColor();
    }
}  // namespace AnEngine::Crank
