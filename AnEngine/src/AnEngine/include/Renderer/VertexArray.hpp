#ifndef VERTEXARRAY_HPP
#define VERTEXARRAY_HPP

#include <memory>

#include "Renderer/Buffers/IndexBuffer.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"


namespace AnEngine {
    class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

        virtual void addVertexBuffer(const Ref<VertexBuffer>& layout) = 0;
        virtual void setIndexBuffer(const Ref<IndexBuffer>& layout) = 0;

        virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers()
            const = 0;
        virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;

        static Ref<VertexArray> create();
    };
};  // namespace AnEngine

#endif
