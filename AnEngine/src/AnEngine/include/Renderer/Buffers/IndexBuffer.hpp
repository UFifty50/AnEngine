#ifndef INDEXBUFFER_HPP
#define INDEXBUFFER_HPP

#include <vector>

#include "Core/Core.hpp"


namespace AnEngine {
    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        virtual uint32_t getCount() const = 0;

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

        static Ref<IndexBuffer> create(const Scope<uint32_t[]>& indices, uint32_t count);
    };
}  // namespace AnEngine

#endif
