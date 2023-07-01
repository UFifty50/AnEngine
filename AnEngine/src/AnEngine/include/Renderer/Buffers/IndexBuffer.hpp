#ifndef INDEXBUFFER_HPP
#define INDEXBUFFER_HPP

#include <vector>


namespace AnEngine {
    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        virtual uint32_t getCount() const = 0;

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

        static IndexBuffer* create(uint32_t* indices, uint32_t count);
        static IndexBuffer* create(std::vector<uint32_t> indices);
        template<uint32_t _Size> static IndexBuffer* create(std::array<uint32_t, _Size> indices);
    };
}

#endif
