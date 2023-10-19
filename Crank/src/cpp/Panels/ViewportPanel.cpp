#include "Panels/ViewportPanel.hpp"

#include "Panels/Panel.hpp"


namespace AnEngine::Crank {
    ViewportPanel::ViewportPanel(std::string name, const Ref<FrameBuffer>& fbuf,
                                 DockSpace& dspace)
        : name(name), frameBuffer(fbuf), dockSpace(dspace) {}

    void ViewportPanel::begin() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::Begin(name.c_str());
    }

    void ViewportPanel::render() {
        uint32_t texID = frameBuffer->getColorAttachmentID();

        ImGui::Image((void*)texID, dockSpace.getViewportSize(), {0, 1}, {1, 0});

        dockSpace.updateViewportInfo();
    }

    void ViewportPanel::end() {
        ImGui::End();
        ImGui::PopStyleVar();
    }
}  // namespace AnEngine::Crank
