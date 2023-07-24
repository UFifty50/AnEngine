#include "aepch.hpp"

#include "Platform/OpenGL/Buffers/OpenGLVertexBuffer.hpp"

#include <glad/glad.h>

#include "Renderer/Buffers/BufferLayout.hpp"


namespace AnEngine {
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, size_t size) : size(size) {
        glCreateBuffers(1, &rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &rendererID); }

    size_t OpenGLVertexBuffer::getSize() const { return this->size; }

    void OpenGLVertexBuffer::bind() const {
        AE_PROFILE_FUNCTION()

        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
    }

    void OpenGLVertexBuffer::unBind() const {
        AE_PROFILE_FUNCTION()

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}  // namespace AnEngine
