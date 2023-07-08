#ifndef OPENGL_VERTEXARRAY_HPP
#define OPENGL_VERTEXARRAY_HPP

#include <glad/glad.h>

#include <memory>
#include <vector>

#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        inline virtual void bind() const override {
            glBindVertexArray(this->rendererID);
        }
        inline virtual void unBind() const override { glBindVertexArray(0); }

        virtual void addVertexBuffer(const Ref<VertexBuffer>& layout) override;
        virtual void setIndexBuffer(const Ref<IndexBuffer>& layout) override;

        inline virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers()
            const override {
            return this->vertexBuffers;
        }
        inline virtual const Ref<IndexBuffer>& getIndexBuffer() const override {
            return this->indexBuffer;
        }

    private:
        std::vector<Ref<VertexBuffer>> vertexBuffers;
        Ref<IndexBuffer> indexBuffer;
        uint32_t rendererID;
    };

    static const GLenum toOpenGLBaseType(ShaderDataType::T type);
}  // namespace AnEngine

#endif
