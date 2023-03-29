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
    }
    
    void OpenGLContext::swapBuffers() {
        glfwSwapBuffers(winPtr);
    }
}
