#ifndef PANEL_HPP
#define PANEL_HPP

#include <imgui.h>

#include <string>


namespace AnEngine::Crank {
    class Panel {
    public:
        virtual ~Panel() = default;

        virtual ImGuiWindowFlags beforeRender() = 0;
        virtual void render() = 0;
        virtual void afterRender() = 0;

        virtual void onClose() = 0;

        virtual std::string getName() = 0;
    };
}  // namespace AnEngine::Crank

#endif
