#include "Panels/ContentBrowserPanel.hpp"

#include <imgui.h>

#include <filesystem>


namespace fs = std::filesystem;

namespace AnEngine::Crank {
    void ContentBrowserPanel::render() {
        ImGui::Text(currentPath.string().c_str());

        if (currentPath != baseAssetsDirectory) {
            if (ImGui::Button("<- Back")) {
                currentPath = currentPath.parent_path();
            }
        }

        for (auto& dirEnt : fs::directory_iterator(currentPath)) {
            const fs::path& path = dirEnt.path();

            fs::path relPath = fs::relative(path, baseAssetsDirectory);
            std::string relPathName = relPath.filename().string();

            if (!dirEnt.is_directory()) {
                ImGui::Text(relPathName.c_str());
                continue;
            }

            if (ImGui::Button(relPathName.c_str())) {
                currentPath = currentPath / path.filename();
            }
        }
    }
}  // namespace AnEngine::Crank
