#include "Menus/FileMenu.hpp"

#include <imgui.h>

#include <string>

#include "Application.hpp"
#include "Core/Core.hpp"
#include "Core/Utils/PlatformUtils.hpp"
#include "Dockspace.hpp"
#include "Scene/Scene.hpp"
#include "Scene/SceneSerialiser.hpp"


namespace AnEngine::Crank {
    void FileMenu::renderMenu() {
        if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
            NewScene(scenesPanel, dockSapce);
        }

        if (ImGui::MenuItem("Open", "Ctrl+O")) {
            OpenScene(scenesPanel, dockSapce);
        }

        if (ImGui::MenuItem("Save As", "CTRL+Shift+S")) {
            SaveScene(scene);
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
    Ref<Scene> FileMenu::NewScene(Ref<ScenesPanel>& spRef, const Ref<DockSpace>& dsRef) {
        Ref<Scene> scene = MakeRef<Scene>();
        scene->onResize((uint32_t)dsRef->getViewportSize().x,
                        (uint32_t)dsRef->getViewportSize().y);
        spRef->setCurrentScene(scene);

        return scene;
    }

    std::optional<Ref<Scene>> FileMenu::OpenScene(const fs::path& path,
                                                  Ref<ScenesPanel>& spRef,
                                                  const Ref<DockSpace>& dsRef) {
        if (!fs::exists(path)) {
            AE_CORE_WARN("File does not exist!");
            return std::nullopt;
        }

        if (fs::is_directory(path)) {
            AE_CORE_WARN("File is a directory!");
            return std::nullopt;
        }

        Ref<Scene> scene = MakeRef<Scene>();
        scene->onResize((uint32_t)dsRef->getViewportSize().x,
                        (uint32_t)dsRef->getViewportSize().y);
        spRef->setCurrentScene(scene);

        SceneSerialiser serialiser(scene);
        if (serialiser.deserialise(path.string())) return scene;

        return std::nullopt;
    }

    std::optional<Ref<Scene>> FileMenu::OpenScene(Ref<ScenesPanel>& spRef,
                                                  const Ref<DockSpace>& dsRef) {
        if (auto path = Dialogues::OpenFileDialogue(
                "CrankEngine Scene (*.aescene)\0*.aescene\0")) {
            return OpenScene(*path, spRef, dsRef);
        }

        return std::nullopt;
    }

    void FileMenu::SaveScene(Ref<Scene>& sceneRef) {
        if (auto path = Dialogues::SaveFileDialogue(
                "CrankEngine Scene (*.aescene)\0 *.aescene\0")) {
            SceneSerialiser serialiser(sceneRef);
            serialiser.serialise(*path);
        }
    }
}  // namespace AnEngine::Crank
