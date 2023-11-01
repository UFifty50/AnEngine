#ifndef OGLINDEXBUFFER_HPP
#define OGLINDEXBUFFER_HPP

#include "Core/Core.hpp"
#include "Renderer/Buffers/IndexBuffer.hpp"


namespace AnEngine {
    class OpenGLIndexBuffer : public IndexBuffer {
    private:
        RenderID rendererID;
        uint32_t count;

    public:
        OpenGLIndexBuffer(const Scope<uint32_t[]>& indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual uint32_t getCount() const override;

        virtual void bind() const override;
        virtual void unBind() const override;
    };
}  // namespace AnEngine

#endif
