#ifndef FILEMENU_HPP
#define FILEMENU_HPP

#include <filesystem>
#include <string>

#include "Core/Core.hpp"
#include "Dockspace.hpp"
#include "Menus/Menu.hpp"
#include "Panels/ScenesPanel.hpp"
#include "Scene/Scene.hpp"


namespace fs = std::filesystem;

namespace AnEngine::Crank {
    class FileMenu : public Menu {
    public:
        FileMenu() = default;

        FileMenu(const std::string& name, Ref<ScenesPanel>& sp, Ref<Scene>& sceneRef,
                 const Ref<DockSpace>& dockSpace)
            : name(name), scenesPanel(sp), scene(sceneRef), dp(dockSpace) {}

        virtual void renderMenu() override;
        virtual void renderMenuPopups() override {}

        virtual std::string getMenuName() override { return name; }


        static void NewScene(Ref<ScenesPanel>& spRef, Ref<Scene>& sceneRef,
                             const Ref<DockSpace>& dsRef);

        static bool OpenScene(Ref<ScenesPanel>& spRef, Ref<Scene>& sceneRef,
                              const Ref<DockSpace>& dsRef);

        static bool OpenScene(const fs::path& path, Ref<Scene>& scene);

        static void SaveScene(Ref<Scene>& sceneRef);

    private:
        std::string name;
        Ref<ScenesPanel> scenesPanel;
        Ref<Scene> scene;
        Ref<DockSpace> dp;

        bool loadSceneOpen = false;
        bool saveSceneOpen = false;
    };
}  // namespace AnEngine::Crank

#endif
