#include "Panels/StatisticsPanel.hpp"

#include "imgui.h"


namespace AnEngine::Crank {
    void StatisticsPanel::render() {
        AE_PROFILE_FUNCTION()

        ImGui::Text("Editor Stats:");
        ImGui::Text("Hovered Entity: %s",
                    hoveredEntity.hasComponent<TagComponent>()
                        ? hoveredEntity.getComponent<TagComponent>().Tag.c_str()
                        : "None");

        ImGui::Separator();

        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("FrameTime: %.2fms", Renderer2D::getStats().lastFrameTime);
        ImGui::Text("FPS: %.1f", 1000.0f / Renderer2D::getStats().lastFrameTime);
        ImGui::Text("Draw Calls: %d", Renderer2D::getStats().draws);
        ImGui::Text("Used Materials: %d", Renderer2D::getStats().usedMaterials);
        ImGui::Text("Quads: %d", Renderer2D::getStats().quadCount);
        ImGui::Text("Vertices: %d", Renderer2D::getStats().getTotalVertexCount());
        ImGui::Text("Indices: %d", Renderer2D::getStats().getTotalIndexCount());
    }
}  // namespace AnEngine::Crank
