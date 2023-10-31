#ifndef VIEWPORTPANEL_HPP
#define VIEWPORTPANEL_HPP

#include <string>

#include "Dockspace.hpp"
#include "Panels/Panel.hpp"
#include "Renderer/FrameBuffer.hpp"


namespace AnEngine::Crank {
    class ViewportPanel : public Panel {
    public:
        ViewportPanel() = default;
        ViewportPanel(std::string name, const Ref<FrameBuffer>& fbuf,
                      Ref<DockSpace>& dspace);

        virtual void beforeRender() override;
        virtual void render() override;
        virtual void afterRender() override;

        virtual void onClose() override {}

        virtual std::string getName() override { return name; }

    private:
        std::string name;
        const Ref<FrameBuffer>& frameBuffer;
        Ref<DockSpace>& dockSpace;
    };
}  // namespace AnEngine::Crank

#endif
