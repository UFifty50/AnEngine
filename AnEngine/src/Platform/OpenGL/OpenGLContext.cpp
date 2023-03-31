#include "aepch.hpp"
#include "OpenGLContext.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace AnEngine {
    OpenGLContext::OpenGLContext(GLFWwindow* winPtr) : winPtr(winPtr) {
        AE_CORE_ASSERT(winPtr, "Window pointer is null!");
    }
    
    void OpenGLContext::init() {
        glfwMakeContextCurrent(winPtr);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        AE_CORE_ASSERT(status, "Failed to initialise Glad!");

        AE_CORE_DEBUG("OpenGL Info:");
        AE_CORE_DEBUG("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
        AE_CORE_DEBUG("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
        AE_CORE_DEBUG("  Version: {0}", (char*)glGetString(GL_VERSION));
        AE_CORE_DEBUG("  GLSL Version: {0}", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    }
    
    void OpenGLContext::swapBuffers() {
        glfwSwapBuffers(winPtr);
    }
}
