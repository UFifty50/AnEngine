#include "aepch.hpp"

#include "Platform/Linux/input/LinuxInput.hpp"

#include <GLFW/glfw3.h>

#include "Application.hpp"


namespace AnEngine {
    Scope<Input> Input::instance = std::make_unique<LinuxInput>();

    bool LinuxInput::isKeyPressedImpl(int keycode) {
        auto window =
            static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        int state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool LinuxInput::isMouseButtonPressedImpl(int button) {
        auto window =
            static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        int state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> LinuxInput::getMousePositionImpl() {
        double xpos, ypos;
        auto window =
            static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        glfwGetCursorPos(window, &xpos, &ypos);
        return {(float)xpos, (float)ypos};
    }

    float LinuxInput::getMouseXImpl() {
        auto [x, y] = getMousePositionImpl();
        return x;
    }

    float LinuxInput::getMouseYImpl() {
        auto [x, y] = getMousePositionImpl();
        return y;
    }
}  // namespace AnEngine
