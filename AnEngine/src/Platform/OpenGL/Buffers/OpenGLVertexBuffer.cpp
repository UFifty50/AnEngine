#include "aepch.hpp"

#include <glad/glad.h>

#include "Platform/OpenGL/Buffers/OpenGLVertexBuffer.hpp"
#include "Renderer/Buffers/BufferLayout.hpp"


namespace AnEngine {
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, size_t size) : size(size) {
        glCreateBuffers(1, &rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        glDeleteBuffers(1, &rendererID);
    }

    size_t OpenGLVertexBuffer::getSize() const {
        return this->size;
    }

    void OpenGLVertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
    }

    void OpenGLVertexBuffer::unBind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
