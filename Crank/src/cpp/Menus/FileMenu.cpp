#include "Menus/FileMenu.hpp"

#include <imgui.h>

#include <string>

#include "Application.hpp"
#include "Core/Core.hpp"
#include "Core/Utils/PlatformUtils.hpp"
#include "Dockspace.hpp"
#include "Globals.hpp"
#include "Scene/Scene2D.hpp"
#include "Scene/Scene3D.hpp"
#include "Scene/SceneSerialiser.hpp"


namespace AnEngine::Crank {
    void FileMenu::renderMenu() {
        if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
            NewScene();
        }

        if (ImGui::MenuItem("Open", "Ctrl+O")) {
            if (!OpenSceneMenu()) AE_CORE_WARN("Failed to open scene!");
        }

        if (ImGui::MenuItem("Save As", "CTRL+Shift+S")) {
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

    void FileMenu::NewScene() {
        g_ActiveScene = MakeRef<Scene2D>();
        g_ActiveScene->onResize((uint32_t)g_DockSpace->getViewportSize().x,
                                (uint32_t)g_DockSpace->getViewportSize().y);
        gPanel_SceneHierarchy->updateCurrentSceneFromActive();
    }

    bool FileMenu::OpenScene(const fs::path& path) {
        if (!fs::exists(path)) {
            AE_CORE_WARN("File does not exist!");
            return false;
        }

        if (fs::is_directory(path)) {
            AE_CORE_WARN("File is a directory!");
            return false;
        }

        if (!SceneSerialiser::isSceneFile(path.string())) {
            AE_CORE_WARN("File is not a scene file!");
            return false;
        }

        NewScene();
        SceneSerialiser serialiser(g_ActiveScene);
        if (serialiser.deserialise(path.string())) return true;

        return false;
    }

    bool FileMenu::OpenSceneMenu() {
        if (auto path = Dialogues::OpenFileDialogue(
                "CrankEngine Scene (*.aescene)\0*.aescene\0")) {
            return OpenScene(*path);
        }

        return false;
    }

    void FileMenu::SaveActiveScene() {
        if (auto path = Dialogues::SaveFileDialogue(
                "CrankEngine Scene (*.aescene)\0 *.aescene\0")) {
            SceneSerialiser serialiser(g_ActiveScene);
            serialiser.serialise(*path);
        }
    }

    void FileMenu::SaveScene(const Ref<Scene>& scene) {
        if (auto path = Dialogues::SaveFileDialogue(
                "CrankEngine Scene (*.aescene)\0 *.aescene\0")) {
            SceneSerialiser serialiser(scene);
            serialiser.serialise(*path);
        }
    }
}  // namespace AnEngine::Crank
