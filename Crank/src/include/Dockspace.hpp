#ifndef DOCKSPACE_HPP
#define DOCKSPACE_HPP

#include <glm/glm.hpp>

#include <functional>
#include <variant>

#include "imgui.h"
#include "imgui_internal.h"

#include "Core/Core.hpp"
#include "Menus/Menu.hpp"
#include "Panels/Panel.hpp"


namespace AnEngine::Crank {
    static bool dockspaceOpen = true;
    static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

    class DockSpace {
    public:
        DockSpace() = default;
        ~DockSpace() = default;

        void addPanel(Ref<Panel> panel) {
            panels.push_back(panel);
            usablePanels.push_back(panel);
        }

        void addMenubarMenu(Ref<Menu> menu) { menus.push_back(menu); }

        void render();

        // Must be called before getMousePosInViewport()
        void updateViewportInfo(bool hasTabBar, bool hasMenuBar);
        bool isMouseInViewport();
        ImVec2 getMousePosInViewport(bool flipY);

        ImVec2 getViewportPos() { return viewportPos; }
        ImVec2 getViewportSize() { return viewportSize; }
        bool isViewportFocused() { return viewportFocused; }
        bool isViewportHovered() { return viewportHovered; }

        ImVec2 getWindowPos() { return windowPos; }
        ImVec2 getWindowSize() { return windowSize; }
        bool isWindowFocused() { return windowFocused; }
        bool isWindowHovered() { return windowHovered; }

    private:
        std::vector<Ref<Menu>> menus;
        std::vector<Ref<Panel>> panels;
        std::vector<Ref<Panel>> usablePanels;

        bool viewportFocused = false;
        bool viewportHovered = false;
        bool windowFocused = false;
        bool windowHovered = false;
        ImVec2 viewportPos;
        ImVec2 viewportSize;
        ImVec2 windowPos;
        ImVec2 windowSize;
    };
}  // namespace AnEngine::Crank
#endif
