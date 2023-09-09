#ifndef SANDBOX2D_HPP
#define SANDBOX2D_HPP

#include <glm/glm.hpp>

#include <functional>

#include "imgui.h"
#include "imgui_internal.h"

#include "Application.hpp"
#include "Core/Core.hpp"
#include "Renderer/Camera/CameraController.hpp"
#include "Renderer/FrameBuffer.hpp"


namespace AnEngine {
    static bool dockspaceOpen = true;
    static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

    class DockSpace {
    public:
        DockSpace() {}
        ~DockSpace() = default;

        void addWindow(std::function<void(void)> fn) { windows.push_back(fn); }

        void render() {
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
            ImGui::Begin("DockSpace Demo", &dockspaceOpen, windowFlags);
            ImGui::PopStyleVar();

            ImGui::PopStyleVar(2);

            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
                ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
            }

            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("Exit", "ALT+F4")) Application::get().Shutdown();
                    /*
                    if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                    if (ImGui::MenuItem("Redo", "CTRL+Y")) {}
                    if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                    if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                    if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                    */
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            for (auto& window : windows) {
                window();
            }

            ImGui::End();
        }

    private:
        std::vector<std::function<void(void)>> windows;
    };


    class CrankEditor : public Layer {
    public:
        CrankEditor();
        virtual ~CrankEditor() = default;

        virtual void onAttach() override;
        virtual void onDetach() override;

        virtual void onUpdate(TimeStep deltaTime) override;
        virtual void onImGuiRender() override;
        virtual void onEvent(Event& event) override;

    private:
        CameraController cameraController;
        Ref<FrameBuffer> frameBuffer;


        DockSpace dockSpace;
    };
}  // namespace AnEngine

#endif
