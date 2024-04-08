#include <Project/Resources/Scene/Scene3D.hpp>

#include "Menus/FileMenu.hpp"

#include <imgui.h>

#include <string>

#include "Application.hpp"
#include "Core/Core.hpp"
#include "Core/Utils/PlatformUtils.hpp"
#include "Dockspace.hpp"
#include "Globals.hpp"
#include "Project/ProjectSerialiser.hpp"
#include "Project/Resources/Scene/Scene2D.hpp"
#include "Project/Resources/Scene/Scene3D.hpp"


namespace AnEngine::Crank {
    void FileMenu::renderMenu() {
        AE_PROFILE_FUNCTION()

        if (ImGui::MenuItem("New Projct")) NewProject();

        if (ImGui::MenuItem("Open Project")) OpenProject();

        if (ImGui::MenuItem("Save Project")) SaveProject();

        ImGui::Separator();

        if (ImGui::MenuItem("New Scene")) {
            NewScene();
        }

        if (ImGui::MenuItem("Open Scene")) {
            if (!OpenSceneMenu()) AE_CORE_WARN("Failed to open scene!");
        }

        if (ImGui::MenuItem("Save Active Scene")) {
            SaveActiveScene();
        }

        if (ImGui::MenuItem("Save UI Layout"))
            Application::saveUILayout("assets/layouts/CrankEditorLayout.ini");

        ImGui::Separator();

        if (ImGui::MenuItem("Exit", "ALT+F4")) Application::Shutdown();
        /*
        if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
        if (ImGui::MenuItem("Redo", "CTRL+Y")) {}
        */
    }

    void FileMenu::NewProject() {
        AE_PROFILE_FUNCTION()
        g_ActiveProject = Project::newProject("");
    }

    bool FileMenu::OpenProjectMenu() {
        AE_PROFILE_FUNCTION()

        if (auto path = Dialogues::OpenFileDialogue(
                "CrankEngine Project (*.aeproj)\0*.aeproj\0")) {
            OpenProject(*path);
            return true;
        }

        return false;
    }

    void FileMenu::OpenProject(const fs::path& path) {
        AE_PROFILE_FUNCTION()

        if (!fs::exists(path)) {
            AE_CORE_WARN("File does not exist!");
            return;
        }

        if (fs::is_directory(path)) {
            AE_CORE_WARN("File is a directory!");
            return;
        }

        ProjectSerialiser serialiser;
        g_ActiveProject = serialiser.openProject(path);
    }

    void FileMenu::SaveProject() {
        AE_PROFILE_FUNCTION()

        if (g_ActiveProject) {
            ProjectSerialiser serialiser(g_ActiveProject);
            serialiser.serialise();
        }
    }

    void FileMenu::NewScene() {
        AE_PROFILE_FUNCTION()

        // TODO: ask to save active scene!
        g_ActiveProject.newScene(false);
        g_ActiveProject.getActiveScene<Scene2D>().onResize(
            (uint32_t)g_DockSpace->getViewportSize().x,
            (uint32_t)g_DockSpace->getViewportSize().y);
        gPanel_SceneHierarchy->updateCurrentSceneFromActive();
    }

    bool FileMenu::OpenScene(const fs::path& path) {
        AE_PROFILE_FUNCTION()

        if (!fs::exists(path)) {
            AE_CORE_WARN("File does not exist!");
            return false;
        }

        if (fs::is_directory(path)) {
            AE_CORE_WARN("File is a directory!");
            return false;
        }

        NewScene();
        if (g_ActiveProject.isPathInProject(path)) g_ActiveProject.loadResourcePath(path);

        ProjectSerialiser serialiser;
        if (auto res = serialiser.openResource(path.string())) {
            g_ActiveProject.addResource(res);
            g_ActiveProject.openScene((*res).uuid);
        }

        return false;
    }

    bool FileMenu::OpenSceneMenu() {
        AE_PROFILE_FUNCTION()

        if (auto path = Dialogues::OpenFileDialogue(
                "CrankEngine Scene (*.aescene)\0*.aescene\0")) {
            return OpenScene(*path);
        }

        return false;
    }

    void FileMenu::SaveActiveScene() {
        AE_PROFILE_FUNCTION()
        ProjectSerialiser serialiser;
        serialiser.saveResource(activeScene);
    }
}  // namespace AnEngine::Crank
