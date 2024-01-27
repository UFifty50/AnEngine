#ifndef VIEWPORTPANEL_HPP
#define VIEWPORTPANEL_HPP

#include <string>

#include "Dockspace.hpp"
#include "Panels/Panel.hpp"
#include "Panels/ScenesPanel.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
#include "Renderer/FrameBuffer.hpp"


namespace AnEngine::Crank {
    class ViewportPanel : public Panel {
    public:
        ViewportPanel() = default;
        ViewportPanel(const std::string& name, const Ref<FrameBuffer>& fbuf,
                      const Ref<EditorCamera>& editorCamera);

        virtual ImGuiWindowFlags beforeRender() override;
        virtual void render() override;
        virtual void afterRender() override;

        virtual void onClose() override {}

        virtual std::string getName() override { return name; }

    private:
        std::string name;

        int32_t gizmoType;
        float translateSnap;
        float rotateSnap;

        Ref<FrameBuffer> frameBuffer;
        Ref<EditorCamera> editorCamera;
    };
}  // namespace AnEngine::Crank

#endif
