#ifndef SCENESPANEL_HPP
#define SCENESPANEL_HPP

#include "Panels/Panel.hpp"


namespace AnEngine::Crank {
    class ScenesPanel : public Panel {
    public:
        ScenesPanel(std::string name);

        virtual void begin() override;
        virtual void render() override;
        virtual void end() override;

        virtual std::string getName() override { return name; }

    private:
        std::string name;
    };
};  // namespace AnEngine::Crank

#endif
