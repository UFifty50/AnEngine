#include "aepch.hpp"

#include <glm/glm.hpp>

#include "Platform/Windows/input/WinInput.hpp"

#include <GLFW/glfw3.h>

#include "Application.hpp"


namespace AnEngine {
    Scope<Input> Input::instance = MakeScope<WinInput>();

    bool WinInput::isKeyPressedImpl(int keycode) {
        auto window =
            static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        int state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WinInput::isMouseButtonPressedImpl(int button) {
        auto window =
            static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        int state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    glm::vec2 WinInput::getMousePositionImpl() {
        double xpos, ypos;
        auto window =
            static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        glfwGetCursorPos(window, &xpos, &ypos);
        return {(float)xpos, (float)ypos};
    }

    float WinInput::getMouseXImpl() { return getMousePositionImpl().x; }

    float WinInput::getMouseYImpl() { return getMousePositionImpl().y; }
}  // namespace AnEngine
