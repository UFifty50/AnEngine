#ifndef OPENGL_UNIFORM_BUFFER_HPP
#define OPENGL_UNIFORM_BUFFER_HPP

#include "Renderer/Buffers/UniformBuffer.hpp"


namespace AnEngine {
    class OpenGLUniformBuffer : public UniformBuffer {
    public:
        OpenGLUniformBuffer(uint32_t size, uint32_t binding);
        virtual ~OpenGLUniformBuffer();

        virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) override;

    private:
        uint32_t rendererID;
    };
}  // namespace AnEngine

#endif
