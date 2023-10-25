#ifndef STATISTICSPANEL_HPP
#define STATISTICSPANEL_HPP

#include <string>

#include "CrankEditor.hpp"
#include "Dockspace.hpp"
#include "Panels/Panel.hpp"
#include "Renderer/Renderer2D.hpp"


namespace AnEngine::Crank {
    class StatisticsPanel : public Panel {
    public:
        StatisticsPanel(std::string name);

        virtual void beforeRender() override {}
        virtual void render() override;
        virtual void afterRender() override {}

        virtual std::string getName() override { return name; }

    private:
        std::string name;
    };
}  // namespace AnEngine::Crank

#endif
