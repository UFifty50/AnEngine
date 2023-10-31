#include "Panels/ViewportPanel.hpp"

#include "Application.hpp"
#include "Panels/Panel.hpp"


namespace AnEngine::Crank {
    ViewportPanel::ViewportPanel(std::string name, const Ref<FrameBuffer>& fbuf,
                                 Ref<DockSpace>& dspace)
        : name(name), frameBuffer(fbuf), dockSpace(dspace) {}

    void ViewportPanel::beforeRender() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    }

    void ViewportPanel::render() {
        dockSpace->updateViewportInfo();
        Application::getImGuiLayer()->shouldAllowEvents(!dockSpace->isViewportFocused() ||
                                                        !dockSpace->isViewportHovered());

        uint32_t texID = frameBuffer->getColorAttachmentID();
        ImGui::Image((void*)texID, dockSpace->getViewportSize(), {0, 1}, {1, 0});
    }

    void ViewportPanel::afterRender() { ImGui::PopStyleVar(); }
}  // namespace AnEngine::Crank
