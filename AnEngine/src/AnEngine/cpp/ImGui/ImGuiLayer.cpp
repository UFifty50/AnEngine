#include "aepch.hpp"

#include "ImGui/ImGuiLayer.hpp"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "ImGuizmo.h"

#include "Application.hpp"
#include "Events/Event.hpp"


namespace AnEngine {
    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

    ImGuiLayer::~ImGuiLayer() = default;

    void ImGuiLayer::onAttach() {
        // setup context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        io.FontDefault = io.Fonts->AddFontFromFileTTF(
            "assets/fonts/SometypeMono/SometypeMono-Medium.ttf",
            Window::HighDPIScaleFactor * 16.0f);
        io.Fonts->AddFontFromFileTTF("assets/fonts/SometypeMono/SometypeMono-Bold.ttf",
                                     Window::HighDPIScaleFactor * 20.0f);

        // set style
        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(Window::HighDPIScaleFactor);

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        useDarkThemeColours();

        auto window =
            static_cast<GLFWwindow*>(Application::getWindow().getNativeWindow());

        // platform/renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void ImGuiLayer::onDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::onEvent(Event& event) {
        if (!allowEvents) {
            ImGuiIO& io = ImGui::GetIO();
            event.handled |= event.isInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            event.handled |=
                event.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

    void ImGuiLayer::begin() {
        if (newConfigQueued) {
            loadConfig(configPath);
            newConfigQueued = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::end() {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)Application::getWindow().getWidth(),
                                (float)Application::getWindow().getHeight());

        // rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backupContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupContext);
        }
    }

    void ImGuiLayer::useDarkThemeColours() {
        auto& colours = ImGui::GetStyle().Colors;
        colours[ImGuiCol_WindowBg] = ImVec4{0.137f, 0.149f, 0.157f, 1.0f};

        // Headers
        colours[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colours[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colours[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Buttons
        colours[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colours[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colours[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Frame BG
        colours[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colours[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colours[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Tabs
        colours[ImGuiCol_Tab] = ImVec4{0.102f, 0.396f, 0.604f, 1.0f};
        colours[ImGuiCol_TabHovered] = ImVec4{0.05f, 0.556f, 0.909f, 1.0f};
        colours[ImGuiCol_TabActive] = ImVec4{0.195f, 0.528f, 0.764f, 1.0f};
        colours[ImGuiCol_TabUnfocused] = ImVec4{0.152f, 0.396f, 0.604f, 1.0f};
        colours[ImGuiCol_TabUnfocusedActive] = ImVec4{0.202f, 0.446f, 0.654f, 1.0f};

        // Title
        colours[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colours[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colours[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    }

    void ImGuiLayer::loadConfig(const std::string& path) {
        ImGui::LoadIniSettingsFromDisk(path.c_str());
    }

    void ImGuiLayer::saveConfig(const std::string& path) const {
        ImGui::SaveIniSettingsToDisk(path.c_str());
    }

    void ImGuiLayer::onImGuiRender() {
        static bool show = true;
        //   ImGui::ShowDemoWindow(&show);
    }
}  // namespace AnEngine
