#ifndef OGLVERTEXBUFFER_HPP
#define OGLVERTEXBUFFER_HPP

#include "Renderer/Buffers/BufferLayout.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"


namespace AnEngine {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        OpenGLVertexBuffer(uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void bind() const override;
        virtual void unBind() const override;

        virtual void setLayout(const BufferLayout& layout) override {
            this->layout = layout;
        }
        virtual const BufferLayout& getLayout() const override { return layout; }

        virtual void setData(const void* data, uint32_t size) override;

    private:
        BufferLayout layout;
        RenderID rendererID;
    };
}  // namespace AnEngine

#endif
