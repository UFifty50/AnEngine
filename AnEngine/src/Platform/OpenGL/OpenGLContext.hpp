#ifndef OGLCONTEXT_HPP
#define OGLCONTEXT_HPP

#include "Renderer/RenderContext.hpp"
struct GLFWwindow;


namespace AnEngine {
    class OpenGLContext : public RenderContext {
    private:
        GLFWwindow* winPtr;

    public:
        OpenGLContext(GLFWwindow* winPtr);

        virtual void init() override;
        virtual void swapBuffers() override;
    };
}

#endif
