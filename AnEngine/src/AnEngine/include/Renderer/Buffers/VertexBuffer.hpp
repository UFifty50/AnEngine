#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#include <vector>
#include "Renderer/Buffers/BufferLayout.hpp"


namespace AnEngine {
    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;

        virtual size_t getSize() const = 0;

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

        virtual void setLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& getLayout() const = 0;


        static VertexBuffer* create(float* vertices, size_t size);
        static VertexBuffer* create(std::vector<float> vertices);
        template<size_t _Size> static VertexBuffer* create(std::array<float, _Size> vertices);
    };
}

#endif
