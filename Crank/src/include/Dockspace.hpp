#ifndef DOCKSPACE_HPP
#define DOCKSPACE_HPP

#include <glm/glm.hpp>

#include <functional>
#include <variant>

#include "imgui.h"
#include "imgui_internal.h"

#include "Core/Core.hpp"
#include "Panels/Panel.hpp"
// #include "Scene/Components.hpp"


namespace AnEngine::Crank {
    static bool dockspaceOpen = true;
    static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

    class DockSpace {
    public:
        DockSpace() : panels({}) {}
        ~DockSpace() = default;

        void addPanel(Ref<Panel> panel) {
            panels.push_back(panel);
            usablePanels.push_back(panel);
        }
        void render();

        //    glm::vec2 getMousePosOnRenderedViewport(CameraComponent cameraComponent);
        void updateViewportInfo();

        //       ImVec2 getMousePosInViewport() { return mousePosInViewport; }
        ImVec2 getViewportPos() { return viewportPos; }
        ImVec2 getViewportSize() { return viewportSize; }
        bool isViewportFocused() { return viewportFocused; }
        bool isViewportHovered() { return viewportHovered; }

        ImVec2 getWindowPos() { return windowPos; }
        ImVec2 getWindowSize() { return windowSize; }
        bool isWindowFocused() { return windowFocused; }
        bool isWindowHovered() { return windowHovered; }

    private:
        std::vector<Ref<Panel>> panels;
        std::vector<Ref<Panel>> usablePanels;

        bool viewportFocused = false;
        bool viewportHovered = false;
        bool windowFocused = false;
        bool windowHovered = false;
        //   ImVec2 mousePosInViewport;
        ImVec2 viewportPos;
        ImVec2 viewportSize;
        ImVec2 windowPos;
        ImVec2 windowSize;
    };
}  // namespace AnEngine::Crank
#endif
