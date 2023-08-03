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

        virtual uint32_t getSize() const;

        virtual void bind() const override;
        virtual void unBind() const override;

        inline virtual void setLayout(const BufferLayout& layout) override {
            this->layout = layout;
        }
        inline virtual const BufferLayout& getLayout() const override {
            return this->layout;
        }

        virtual void setData(const void* data, uint32_t size) override;

    private:
        BufferLayout layout;
        uint32_t rendererID;
        uint32_t size;
    };
}  // namespace AnEngine

#endif
