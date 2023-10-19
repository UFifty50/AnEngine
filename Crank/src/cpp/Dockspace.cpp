#include <glm/glm.hpp>

#include "Dockspace.hpp"

#include <functional>

#include "imgui.h"
#include "imgui_internal.h"

#include "Application.hpp"


namespace AnEngine::Crank {
    void DockSpace::render() {
        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                       ImGuiWindowFlags_NoBringToFrontOnFocus |
                       ImGuiWindowFlags_NoNavFocus;

        if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) {
            windowFlags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &dockspaceOpen, windowFlags);
        ImGui::PopStyleVar();

        ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save UI Layout", "CTRL+S"))
                    Application::saveUILayout("assets/layouts/CrankEditorLayout.ini");

                if (ImGui::MenuItem("Exit", "ALT+F4")) Application::Shutdown();
                /*
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y")) {}
                */
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }


        for (auto& panel : panels) {
            panel->begin();
            panel->render();
            panel->end();
        }

        windowPos = ImGui::GetWindowPos();
        windowSize = ImGui::GetWindowSize();
        windowFocused = ImGui::IsWindowFocused();
        windowHovered = ImGui::IsWindowHovered();

        ImGui::End();
    }

    void DockSpace::updateViewportInfo() {
        auto vMin = ImGui::GetWindowContentRegionMin();
        auto vMax = ImGui::GetWindowContentRegionMax();

        viewportSize = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);
        viewportPos = ImGui::GetWindowPos();
        viewportFocused = ImGui::IsItemFocused();
        viewportHovered = ImGui::IsItemHovered();
    }

    // glm::vec2 DockSpace::getMousePosOnRenderedViewport(CameraComponent cameraComponent)
    // {
    //     // updateViewportInfo();

    //    auto camBounds = cameraComponent.Camera.getOrthographicBounds();

    //    float x = (mousePosInViewport.x / viewportSize.x) * camBounds.getWidth() -
    //              camBounds.getWidth() * 0.5f;
    //    float y = camBounds.getHeight() * 0.5f -
    //              (mousePosInViewport.y / viewportSize.y) * camBounds.getHeight();

    //    return {x, y};
    //}
}  // namespace AnEngine::Crank
