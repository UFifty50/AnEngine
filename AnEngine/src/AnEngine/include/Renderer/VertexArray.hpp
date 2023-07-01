#ifndef VERTEXARRAY_HPP
#define VERTEXARRAY_HPP

#include <memory>

#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Renderer/Buffers/IndexBuffer.hpp"


namespace AnEngine {
    class VertexArray {
    public:
        virtual ~VertexArray();

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

        virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& layout) = 0;
        virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& layout) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
        virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const = 0;

        static VertexArray* create();
    };
};

#endif
