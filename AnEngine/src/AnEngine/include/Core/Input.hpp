#ifndef INPUT_HPP
#define INPUT_HPP

#include <glm/glm.hpp>

#include <utility>

#include "Core/Core.hpp"
#include "Core/KeyCodes.hpp"
#include "Core/MouseCodes.hpp"


namespace AnEngine {
    class Input {
    public:
        static bool isKeyPressed(KeyCode keycode);
        static bool isMouseButtonPressed(MouseCode button);
        static glm::vec2 getMousePosition();
        static float getMouseX();
        static float getMouseY();
    };
}  // namespace AnEngine

#endif
