#ifndef DOCKSPACE_HPP
#define DOCKSPACE_HPP

#include <glm/glm.hpp>

#include <functional>
#include <variant>

#include "imgui.h"
#include "imgui_internal.h"

// #include "Scene/Components.hpp"


namespace AnEngine::Crank {
    static bool dockspaceOpen = true;
    static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

    struct DockedWindow {
        const char* name;
        std::function<void(void)> fn;
        std::unordered_map<ImGuiStyleVar, std::variant<float, ImVec2>> styleVars = {};
    };

    class DockSpace {
    public:
        DockSpace() {}
        ~DockSpace() = default;

        void addWindow(DockedWindow window) { windows.push_back(window); }
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
        std::vector<DockedWindow> windows;

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
