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
        ImGui::PopStyleVar(3);


        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
        }

        if (ImGui::BeginMenuBar()) {
            for (auto& menu : menus) {
                std::string name = menu->getMenuName();
                if (name == "Window") continue;

                if (ImGui::BeginMenu(name.c_str())) {
                    menu->renderMenu();
                    ImGui::EndMenu();
                }

                menu->renderMenuPopups();
            }

            if (ImGui::BeginMenu("Window")) {
                for (auto& panel : usablePanels) {
                    if (ImGui::MenuItem(panel->getName().c_str())) {
                        if (std::ranges::find(panels, panel) == panels.end())
                            panels.push_back(panel);
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        bool open;
        for (auto& panel : panels) {
            open = true;

            auto flags = panel->beforeRender();

            ImGui::Begin(panel->getName().c_str(), &open, flags);
            panel->render();
            ImGui::End();

            panel->afterRender();

            if (!open) {
                panel->onClose();
                panels.erase(std::remove(panels.begin(), panels.end(), panel));
                break;
            }
        }

        windowSize = ImGui::GetWindowSize();
        windowPos = ImGui::GetWindowPos();
        windowFocused = ImGui::IsWindowFocused();
        windowHovered = ImGui::IsWindowHovered();

        ImGui::End();
    }

    void DockSpace::updateViewportInfo(bool hasTabBar, bool hasMenuBar) {
        viewportSize = {ImGui::GetContentRegionAvail().x,
                        ImGui::GetContentRegionAvail().y};
        viewportPos = {
            ImGui::GetWindowPos().x,
            ImGui::GetWindowPos().y +
                ImGui::GetCurrentWindow()->MenuBarHeight() * (hasTabBar + hasMenuBar)};

        viewportFocused = ImGui::IsWindowFocused();
        viewportHovered = ImGui::IsWindowHovered();
    }

    bool DockSpace::isMouseInViewport() {
        ImVec2 mousePos = ImGui::GetMousePos();
        return mousePos.x >= viewportPos.x &&
               mousePos.x <= viewportPos.x + viewportSize.x &&
               mousePos.y >= viewportPos.y &&
               mousePos.y <= viewportPos.y + viewportSize.y;
    }

    ImVec2 DockSpace::getMousePosInViewport(bool flipY) {
        ImVec2 mousePos = ImGui::GetMousePos();
        return ImVec2(mousePos.x - viewportPos.x,
                      flipY ? viewportSize.y - (mousePos.y - viewportPos.y)
                            : mousePos.y - viewportPos.y);
    }
}  // namespace AnEngine::Crank
