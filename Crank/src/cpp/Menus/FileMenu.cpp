#include <Project/Resources/Scene/Scene3D.hpp>

#include "Menus/FileMenu.hpp"

#include <imgui.h>

#include <string>

#include "Application.hpp"
#include "Dockspace.hpp"
#include "Globals.hpp"
#include "Core/Core.hpp"
#include "Core/Utils/PlatformUtils.hpp"
#include "Project/ProjectSerialiser.hpp"
#include "Project/Resources/Scene/Scene2D.hpp"
#include "Project/Resources/Scene/Scene3D.hpp"


namespace AnEngine::Crank {
void FileMenu::renderMenu() {
    AE_PROFILE_FUNCTION()

    if (ImGui::MenuItem("New Projct")) NewProject();

    if (ImGui::MenuItem("Open Project")) OpenProjectMenu();

    if (ImGui::MenuItem("Save Project")) SaveProject();

    ImGui::Separator();

    if (ImGui::MenuItem("New Scene")) { NewScene(); }

    if (ImGui::MenuItem("Open Scene")) {
        if (!OpenSceneMenu())
            AE_CORE_WARN("Failed to open scene!");
    }

    if (ImGui::MenuItem("Save Active Scene")) { SaveActiveScene(); }

    if (ImGui::MenuItem("Save UI Layout"))
        Application::saveUILayout(
            "assets/layouts/CrankEditorLayout.ini");

    ImGui::Separator();

    if (ImGui::MenuItem("Exit", "ALT+F4")) Application::Shutdown();
    /*
    if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
    if (ImGui::MenuItem("Redo", "CTRL+Y")) {}
    */
}

void FileMenu::NewProject() {
    AE_PROFILE_FUNCTION()
    g_ActiveProject = Project::NewProject("");
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

    if (!exists(path)) {
        AE_CORE_WARN("File does not exist!");
        return;
    }

    if (is_directory(path)) {
        AE_CORE_WARN("File is a directory!");
        return;
    }

    g_ActiveProject = ProjectSerialiser::DeserialiseProject(path);
}

void FileMenu::SaveProject() {
    AE_PROFILE_FUNCTION()

    //  if (g_ActiveProject) {
    //      ProjectSerialiser serialiser(g_ActiveProject);
    //       serialiser.serialise();
    // }
}

void FileMenu::NewScene() {
    AE_PROFILE_FUNCTION()

    // TODO: ask to save active scene!
    g_ActiveProject.newScene(false);
    auto s = g_ActiveProject.getActiveScene();
    Ref<Scene> sc = s.asScene();
    sc->onResize(
        static_cast<uint32_t>(g_DockSpace->getViewportSize().x),
        static_cast<uint32_t>(g_DockSpace->getViewportSize().y));
    gPanel_SceneHierarchy->updateCurrentSceneFromActive();
}

bool FileMenu::OpenScene(const fs::path& path) {
    AE_PROFILE_FUNCTION()

    if (!exists(path)) {
        AE_CORE_WARN("File does not exist!");
        return false;
    }

    if (is_directory(path)) {
        AE_CORE_WARN("File is a directory!");
        return false;
    }

    if (!g_ActiveProject.isPathInProject(path)) {
        AE_CORE_WARN("File is not in project directory!");
        return false;
    }

    // split path into project root and relative path
    fs::path rootDir = g_ActiveProject.getProjectRootPath();
    fs::path relPath = relative(path, rootDir);

    NewScene();
    Resource sceneRes = ProjectSerialiser::DeserialiseResource(rootDir, relPath);
    g_ActiveProject.addResource(sceneRes, relPath);
    //   g_ActiveProject.openScene(res.uuid);


    return false;
}

bool FileMenu::OpenSceneMenu() {
    AE_PROFILE_FUNCTION()

    if (auto path = Dialogues::OpenFileDialogue(
        "CrankEngine Scene (*.aescene)\0*.aescene\0")) { return OpenScene(*path); }

    return false;
}

void FileMenu::SaveActiveScene() {
    AE_PROFILE_FUNCTION()
    ProjectSerialiser serialiser;
    //     serialiser.saveResource(activeScene);
}
} // namespace AnEngine::Crank
