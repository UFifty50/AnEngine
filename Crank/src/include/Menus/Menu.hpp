#ifndef MENU_HPP
#define MENU_HPP

#include <imgui.h>

#include <string>


namespace AnEngine::Crank {
    class Menu {
    public:
        virtual ~Menu() = default;

        virtual void renderMenu() = 0;
        virtual void renderMenuPopups() = 0;

        virtual std::string getMenuName() = 0;
    };
}  // namespace AnEngine::Crank

#endif
