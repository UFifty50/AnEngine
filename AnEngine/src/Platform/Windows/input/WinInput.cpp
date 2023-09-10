#include "aepch.hpp"

#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

#include "Application.hpp"
#include "Core/Input.hpp"
#include "Core/KeyCodes.hpp"
#include "Core/MouseCodes.hpp"


namespace AnEngine {
    bool Input::isKeyPressed(KeyCode keycode) {
        auto window =
            static_cast<GLFWwindow*>(Application::getWindow().getNativeWindow());
        int state = glfwGetKey(window, keycode.asInt());
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isMouseButtonPressed(MouseCode button) {
        auto window =
            static_cast<GLFWwindow*>(Application::getWindow().getNativeWindow());
        int state = glfwGetMouseButton(window, button.asInt());
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::getMousePosition() {
        double xpos, ypos;
        auto window =
            static_cast<GLFWwindow*>(Application::getWindow().getNativeWindow());
        glfwGetCursorPos(window, &xpos, &ypos);
        return {(float)xpos, (float)ypos};
    }

    float Input::getMouseX() { return getMousePosition().x; }

    float Input::getMouseY() { return getMousePosition().y; }
}  // namespace AnEngine
