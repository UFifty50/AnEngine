#ifndef UNIFORM_BUFFER_HPP
#define UNIFORM_BUFFER_HPP

#include "Core/Core.hpp"


namespace AnEngine {
    class UniformBuffer {
    public:
        virtual ~UniformBuffer() = default;
        virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

        static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
    };
};  // namespace AnEngine

#endif
