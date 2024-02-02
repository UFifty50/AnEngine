#include "aepch.hpp"

#include "Platform/OpenGL/Buffers/OpenGLVertexBuffer.hpp"

#include <glad/glad.h>

#include "Renderer/Buffers/BufferLayout.hpp"


namespace AnEngine {
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
        AE_PROFILE_FUNCTION()

        glCreateBuffers(1, &rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
        AE_PROFILE_FUNCTION()

        glCreateBuffers(1, &rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &rendererID); }

    void OpenGLVertexBuffer::setData(const void* data, uint32_t size) {
        AE_PROFILE_FUNCTION()

        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    void OpenGLVertexBuffer::bind() const {
        AE_PROFILE_FUNCTION()

        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
    }

    void OpenGLVertexBuffer::unBind() const {
        AE_PROFILE_FUNCTION()

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}  // namespace AnEngine
