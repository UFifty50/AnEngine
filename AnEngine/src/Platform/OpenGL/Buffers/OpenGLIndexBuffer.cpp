#include "aepch.hpp"

#include "OpenGLIndexBuffer.hpp"

#include <glad/glad.h>


namespace AnEngine {
    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t const* indices, uint32_t count)
        : count(count) {
        AE_PROFILE_FUNCTION()

        glCreateBuffers(1, &rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }
    OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &rendererID); }

    void OpenGLIndexBuffer::bind() const {
        AE_PROFILE_FUNCTION()

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
    }
    void OpenGLIndexBuffer::unBind() const {
        AE_PROFILE_FUNCTION()

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32_t OpenGLIndexBuffer::getCount() const { return this->count; }
}  // namespace AnEngine
