#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#include <vector>

#include "Core/Core.hpp"
#include "Renderer/Buffers/BufferLayout.hpp"


namespace AnEngine {
    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

        virtual void setLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& getLayout() const = 0;

        virtual void setData(const void* data, uint32_t size) = 0;


        static Ref<VertexBuffer> create(float* vertices, uint32_t size);
        static Ref<VertexBuffer> create(std::vector<float> vertices);

        template <uint32_t _Size>
        static Ref<VertexBuffer> create(std::array<float, _Size> vertices);

        static Ref<VertexBuffer> create(uint32_t size);
    };
}  // namespace AnEngine

#endif
