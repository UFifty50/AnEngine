#ifndef CONTENTBROWSERPANEL_HPP
#define CONTENTBROWSERPANEL_HPP

#include <imgui.h>

#include <filesystem>

#include "Panels/Panel.hpp"


namespace fs = std::filesystem;

namespace AnEngine::Crank {
    static const fs::path baseAssetsDirectory = "assets";  // TODO: add projects

    class ContentBrowserPanel : public Panel {
    public:
        ContentBrowserPanel(std::string name)
            : name(name), currentPath(baseAssetsDirectory) {}

        virtual ImGuiWindowFlags beforeRender() override { return 0; }
        virtual void render() override;
        virtual void afterRender() override {}

        virtual void onClose() override {}

        virtual std::string getName() override { return name; }

    private:
        std::string name;
        fs::path currentPath;
    };
}  // namespace AnEngine::Crank

#endif
