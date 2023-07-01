#ifndef OPENGL_VERTEXARRAY_HPP
#define OPENGL_VERTEXARRAY_HPP

#include <vector>
#include <memory>
#include <glad/glad.h>
#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray() = default;

        inline virtual void bind() const override { glBindVertexArray(this->rendererID); }
        inline virtual void unBind() const override { glBindVertexArray(0); }

        virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& layout) override;
        virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& layout) override;

        inline virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const override { return this->vertexBuffers; }
        inline virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const override { return this->indexBuffer;  }

    private:
        std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
        std::shared_ptr<IndexBuffer> indexBuffer;
        uint32_t rendererID;
    };

    static const GLenum toOpenGLBaseType(ShaderDataType::T type);
}

#endif
