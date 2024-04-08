#ifndef FILEMENU_HPP
#define FILEMENU_HPP

#include <filesystem>
#include <string>

#include "Core/Core.hpp"
#include "Dockspace.hpp"
#include "Menus/Menu.hpp"
#include "Panels/ScenesPanel.hpp"
#include "Project/Resource.hpp"
#include "Project/Resources/Scene/Scene.hpp"


namespace fs = std::filesystem;

namespace AnEngine::Crank {
    class FileMenu : public Menu {
    public:
        FileMenu() = default;

        FileMenu(const std::string& name) : name(name) {}

        virtual void renderMenu() override;
        virtual void renderMenuPopups() override {}

        virtual std::string getMenuName() override { return name; }

        // -- Projects --
        static void NewProject();

        static bool OpenProjectMenu();
        static void OpenProject(const fs::path& path);

        static void SaveProject();

        // -- Scenes --
        static void NewScene();

        static bool OpenSceneMenu();
        static bool OpenScene(const fs::path& path);

        static void SaveActiveScene();

    private:
        std::string name;

        bool loadSceneOpen = false;
        bool saveSceneOpen = false;
    };
}  // namespace AnEngine::Crank

#endif
