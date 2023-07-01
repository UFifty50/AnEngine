#ifndef OGLVERTEXBUFFER_HPP
#define OGLVERTEXBUFFER_HPP

#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Renderer/Buffers/BufferLayout.hpp"


namespace AnEngine {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(float* vertices, size_t size);
        virtual ~OpenGLVertexBuffer();

        virtual size_t getSize() const;

        virtual void bind() const override;
        virtual void unBind() const override;

        inline virtual void setLayout(const BufferLayout& layout) override { this->layout = layout; }
        inline virtual const BufferLayout& getLayout() const override { return this->layout; }

    private:
        BufferLayout layout;
        uint32_t rendererID;
        size_t size;
    };
}

#endif
