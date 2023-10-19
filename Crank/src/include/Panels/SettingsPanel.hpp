#ifndef SETTINGSPANEL_HPP
#define SETTINGSPANEL_HPP

#include "Panels/Panel.hpp"
#include "Scene/Entity.hpp"


namespace AnEngine::Crank {
    class SettingsPanel : public Panel {
    public:
        SettingsPanel(std::string name, Entity& p, Entity& c, Entity& lC, bool& cA);

        virtual void begin() override;
        virtual void render() override;
        virtual void end() override;

        virtual std::string getName() override { return name; }

    private:
        std::string name;
        Entity& player;
        Entity& camera;
        Entity& lockedCamera;
        bool& CameraA;
    };
};  // namespace AnEngine::Crank

#endif
