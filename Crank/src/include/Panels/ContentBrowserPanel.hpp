#ifndef CONTENTBROWSERPANEL_HPP
#define CONTENTBROWSERPANEL_HPP

#include <imgui.h>

#include <filesystem>

#include <Core/UUID.hpp>

#include "Panels/Panel.hpp"
#include "Texture/Texture2D.hpp"


namespace fs = std::filesystem;

namespace AnEngine::Crank {
enum class PayloadType { Scene, Texture, Material };

struct DropPayload {
    //    const Directory path;
    const UUID resourceUUID;
    const PayloadType type;

    ~DropPayload() = default;
};

class ContentBrowserPanel : public Panel {
public:
    ContentBrowserPanel(const std::string& name);

    ImGuiWindowFlags beforeRender() override {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

        return ImGuiWindowFlags_MenuBar;
    }

    void render() override;

    void afterRender() override { ImGui::PopStyleVar(); }

    void onClose() override {}

    std::string getName() override { return name; }

    bool isItemSelected() const { return !selectedResource.isNull(); }
    //    bool isItemSelected() { return !selectedItem.uuid.isNull(); }
    UUID getSelectedItem() const { return selectedResource; }
    //  File getSelectedItem() { return selectedItem; }

private:
    std::string name;
    fs::path currentPath;
    UUID selectedResource;
    //   Directory currentPath;
    // File selectedItem;

    Ref<Texture2D> fileIcon;
    Ref<Texture2D> materialIcon;
    Ref<Texture2D> directoryIcon;

    float thumbSize = 120.0f;
    float paddingWidth = 5.0f;


    const char* labelFromInt(uint32_t i) { return ("##" + std::to_string(i)).c_str(); }
};
} // namespace AnEngine::Crank

#endif
