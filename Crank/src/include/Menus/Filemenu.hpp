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

        FileMenu(const std::string& name) : name(name) {}

        virtual void renderMenu() override;
        virtual void renderMenuPopups() override {}

        virtual std::string getMenuName() override { return name; }


        static void NewScene();

        static bool OpenSceneMenu();
        static bool OpenScene(const fs::path& path);

        static void SaveActiveScene();
        static void SaveScene(const Ref<Scene>& scene);

    private:
        std::string name;

        bool loadSceneOpen = false;
        bool saveSceneOpen = false;
    };
}  // namespace AnEngine::Crank

#endif
