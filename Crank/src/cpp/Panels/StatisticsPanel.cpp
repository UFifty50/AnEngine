#include "Panels/StatisticsPanel.hpp"

#include "imgui.h"


namespace AnEngine::Crank {
    StatisticsPanel::StatisticsPanel(std::string name) : name(name) {}

    void StatisticsPanel::begin() { ImGui::Begin(name.c_str()); }

    void StatisticsPanel::render() {
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("FrameTime: %.2fms", Renderer2D::getStats().lastFrameTime);
        ImGui::Text("FPS: %.1f", 1000.0f / Renderer2D::getStats().lastFrameTime);
        ImGui::Text("Draw Calls: %d", Renderer2D::getStats().draws);
        ImGui::Text("Quads: %d", Renderer2D::getStats().quadCount);
        ImGui::Text("Vertices: %d", Renderer2D::getStats().getTotalVertexCount());
        ImGui::Text("Indices: %d", Renderer2D::getStats().getTotalIndexCount());
    }

    void StatisticsPanel::end() { ImGui::End(); }
}  // namespace AnEngine::Crank
