#include "aepch.hpp"

#include "OpenGLContext.hpp"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    OpenGLContext::OpenGLContext(GLFWwindow* winPtr) : winPtr(winPtr) {
        AE_CORE_ASSERT(winPtr, "Window pointer is null!");
    }

    void OpenGLContext::init() {
        AE_PROFILE_FUNCTION()

        glfwMakeContextCurrent(winPtr);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        AE_CORE_ASSERT(status, "Failed to initialise Glad!");

        AE_CORE_DEBUG("OpenGL Info:");
        AE_CORE_DEBUG("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
        AE_CORE_DEBUG("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
        AE_CORE_DEBUG("  Version: {0}", (char*)glGetString(GL_VERSION));
        AE_CORE_DEBUG("  GLSL Version: {0}",
                      (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        AE_CORE_DEBUG("Current RenderAPI: {0}", RenderAPI::getAPIName());

        AE_CORE_ASSERT(
            GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5),
            "AnEngine requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::swapBuffers() {
        AE_PROFILE_FUNCTION()

        glfwSwapBuffers(winPtr);
    }
}  // namespace AnEngine
