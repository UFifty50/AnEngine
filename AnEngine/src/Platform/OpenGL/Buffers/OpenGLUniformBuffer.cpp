#include "aepch.hpp"

#include "Platform/OpenGL/Buffers/OpenGLUniformBuffer.hpp"

#include <glad/glad.h>


namespace AnEngine {
    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) {
        AE_PROFILE_FUNCTION()

        glCreateBuffers(1, &rendererID);
        glNamedBufferData(rendererID, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, rendererID);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer() { glDeleteBuffers(1, &rendererID); }

    void OpenGLUniformBuffer::setData(const void* data, uint32_t size, uint32_t offset) {
        AE_PROFILE_FUNCTION()

        glNamedBufferSubData(rendererID, offset, size, data);
    }
}  // namespace AnEngine
