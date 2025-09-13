#ifndef STATISTICSPANEL_HPP
#define STATISTICSPANEL_HPP

#include <string>

#include "Panels/Panel.hpp"
#include "Project/Resources/Scene/Entity.hpp"


namespace AnEngine::Crank {
class StatisticsPanel : public Panel {
public:
    StatisticsPanel() = default;
    StatisticsPanel(const std::string& name) : name(name) {}

    ImGuiWindowFlags beforeRender() override { return 0; }
    void render() override;
    void afterRender() override {}

    void onClose() override {}

    std::string getName() override { return name; }

    void setHoveredEntity(Entity entity) { hoveredEntity = entity; }
    Entity getHoveredEntity() { return hoveredEntity; }

private:
    std::string name;
    Entity hoveredEntity;
};
} // namespace AnEngine::Crank

#endif
