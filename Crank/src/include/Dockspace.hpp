#ifndef DOCKSPACE_HPP
#define DOCKSPACE_HPP

#include <glm/glm.hpp>

#include <functional>

#include "imgui.h"
#include "imgui_internal.h"

// #include "Scene/Components.hpp"


namespace AnEngine::Crank {
    static bool dockspaceOpen = true;
    static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

    class DockSpace {
    public:
        DockSpace() {}
        ~DockSpace() = default;

        void addWindow(std::function<void(void)> fn) { windows.push_back(fn); }
        void render();

        //    glm::vec2 getMousePosOnRenderedViewport(CameraComponent cameraComponent);
        void updateViewportInfo();

        ImVec2 getMousePosInViewport() { return mousePosInViewport; }
        ImVec2 getViewportPos() { return viewportPos; }
        ImVec2 getViewportSize() { return viewportSize; }
        bool isViewportFocused() { return viewportFocused; }
        bool isViewportHovered() { return viewportHovered; }

    private:
        std::vector<std::function<void(void)>> windows;

        bool viewportFocused = false;
        bool viewportHovered = false;
        ImVec2 mousePosInViewport;
        ImVec2 viewportPos;
        ImVec2 viewportSize;
    };
}  // namespace AnEngine::Crank
#endif
