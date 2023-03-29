#ifndef RENDERCONTEXT_HPP
#define RENDERCONTEXT_HPP


namespace AnEngine {
    class RenderContext {
    public:
        virtual void init() = 0;
        virtual void swapBuffers() = 0;
    };
}

#endif
