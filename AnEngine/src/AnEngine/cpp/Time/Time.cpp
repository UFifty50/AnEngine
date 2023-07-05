#include "aepch.hpp"

#include "Time/Time.hpp"

#include <GLFW/glfw3.h>


// TODO: Forward to platform specific implementation

namespace AnEngine {
    float AnEngine::Time::getTime() { return (float)glfwGetTime(); }
};  // namespace AnEngine
