#ifndef VIEWPORTPANEL_HPP
#define VIEWPORTPANEL_HPP

#include <string>

#include "CrankEditor.hpp"
#include "Dockspace.hpp"
#include "Panels/Panel.hpp"

namespace AnEngine::Crank {
    class ViewportPanel : public Panel {
    public:
        ViewportPanel(std::string name, const Ref<FrameBuffer>& fbuf, DockSpace& dspace);

        virtual void beforeRender() override;
        virtual void render() override;
        virtual void afterRender() override;

        virtual std::string getName() override { return name; }

    private:
        std::string name;
        const Ref<FrameBuffer>& frameBuffer;
        DockSpace& dockSpace;
    };
}  // namespace AnEngine::Crank

#endif
