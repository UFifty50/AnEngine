#ifndef CONTENTBROWSERPANEL_HPP
#define CONTENTBROWSERPANEL_HPP

#include <imgui.h>

#include <filesystem>

#include "Panels/Panel.hpp"
#include "Texture/Texture2D.hpp"


namespace fs = std::filesystem;

namespace AnEngine::Crank {
    enum class PayloadType { Scene, Texture, Material };

    struct DropPayload {
        const fs::path path;
        const PayloadType type;

        ~DropPayload() = default;
    };

    class ContentBrowserPanel : public Panel {
    public:
        ContentBrowserPanel(const std::string& name);

        virtual ImGuiWindowFlags beforeRender() override {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

            return ImGuiWindowFlags_MenuBar;
        }

        virtual void render() override;

        virtual void afterRender() override { ImGui::PopStyleVar(); }

        virtual void onClose() override {}

        virtual std::string getName() override { return name; }

        bool isItemSelected() { return selectedItem != ""; }
        fs::path getSelectedItem() { return selectedItem; }

    private:
        std::string name;
        fs::path currentPath;
        fs::path selectedItem;

        Ref<Texture2D> fileIcon;
        Ref<Texture2D> materialIcon;
        Ref<Texture2D> directoryIcon;

        float thumbSize = 120.0f;
        float paddingWidth = 5.0f;


        const char* labelFromInt(uint32_t i) {
            return ("##" + std::to_string(i)).c_str();
        }
    };
}  // namespace AnEngine::Crank

#endif
