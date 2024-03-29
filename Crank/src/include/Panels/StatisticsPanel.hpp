#ifndef STATISTICSPANEL_HPP
#define STATISTICSPANEL_HPP

#include <string>

#include "Dockspace.hpp"
#include "Panels/Panel.hpp"
#include "Renderer/Renderer2D.hpp"


namespace AnEngine::Crank {
    class StatisticsPanel : public Panel {
    public:
        StatisticsPanel() = default;
        StatisticsPanel(const std::string& name) : name(name) {}

        virtual ImGuiWindowFlags beforeRender() override { return 0; }
        virtual void render() override;
        virtual void afterRender() override {}

        virtual void onClose() override {}

        virtual std::string getName() override { return name; }

        void setHoveredEntity(Entity entity) { hoveredEntity = entity; }
        Entity getHoveredEntity() { return hoveredEntity; }

    private:
        std::string name;
        Entity hoveredEntity;
    };
}  // namespace AnEngine::Crank

#endif
