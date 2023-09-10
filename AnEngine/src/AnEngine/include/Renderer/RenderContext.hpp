#ifndef RENDERCONTEXT_HPP
#define RENDERCONTEXT_HPP


namespace AnEngine {
    class RenderContext {
    public:
        virtual ~RenderContext() = default;

        virtual void init() = 0;
        virtual void swapBuffers() = 0;
    };
}  // namespace AnEngine

#endif
