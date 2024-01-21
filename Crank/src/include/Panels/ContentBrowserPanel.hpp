#ifndef CONTENTBROWSERPANEL_HPP
#define CONTENTBROWSERPANEL_HPP

#include <imgui.h>

#include <filesystem>

#include "Panels/Panel.hpp"
#include "Texture/Texture2D.hpp"


namespace fs = std::filesystem;

namespace AnEngine::Crank {
    class ContentBrowserPanel : public Panel {
    public:
        ContentBrowserPanel(std::string name);

        virtual ImGuiWindowFlags beforeRender() override {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

            return ImGuiWindowFlags_MenuBar;
        }

        virtual void render() override;

        virtual void afterRender() override { ImGui::PopStyleVar(); }

        virtual void onClose() override {}

        virtual std::string getName() override { return name; }

    private:
        std::string name;
        fs::path currentPath;

        Ref<Texture2D> directoryIcon;
        Ref<Texture2D> fileIcon;

        float thumbSize = 120.0f;
        float paddingWidth = 5.0f;

        const char* labelFromInt(uint32_t i) {
            return ("##" + std::to_string(i)).c_str();
        }
    };
}  // namespace AnEngine::Crank

#endif
