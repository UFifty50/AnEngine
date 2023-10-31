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
            NewScene(scenesPanel, scene, dp);
        }

        if (ImGui::MenuItem("Open", "Ctrl+O")) {
            OpenScene(scenesPanel, scene, dp);
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
    void FileMenu::NewScene(Ref<ScenesPanel>& spRef, Ref<Scene>& sceneRef,
                            const Ref<DockSpace>& dsRef) {
        sceneRef->clear();
        sceneRef->onResize((uint32_t)dsRef->getViewportSize().x,
                           (uint32_t)dsRef->getViewportSize().y);
        spRef->setCurrentScene(sceneRef);
    }

    void FileMenu::OpenScene(Ref<ScenesPanel>& spRef, Ref<Scene>& sceneRef,
                             const Ref<DockSpace>& dsRef) {
        if (auto path = Dialogues::OpenFileDialogue(
                "CrankEngine Scene (*.aescene)\0*.aescene\0")) {
            sceneRef->clear();
            sceneRef->onResize((uint32_t)dsRef->getViewportSize().x,
                               (uint32_t)dsRef->getViewportSize().y);
            spRef->setCurrentScene(sceneRef);

            SceneSerialiser serialiser(sceneRef);
            serialiser.deserialise(*path);
        }
    }
    void FileMenu::SaveScene(Ref<Scene>& sceneRef) {
        if (auto path = Dialogues::SaveFileDialogue(
                "CrankEngine Scene (*.aescene)\0 *.aescene\0")) {
            SceneSerialiser serialiser(sceneRef);
            serialiser.serialise(*path);
        }
    }
}  // namespace AnEngine::Crank
