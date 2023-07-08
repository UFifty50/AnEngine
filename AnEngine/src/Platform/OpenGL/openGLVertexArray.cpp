#include "aepch.hpp"

#include "Platform/OpenGL/openGLVertexArray.hpp"

#include <glad/glad.h>

#include "Renderer/Buffers/BufferLayout.hpp"
#include "Renderer/Buffers/IndexBuffer.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"


namespace AnEngine {
    OpenGLVertexArray::OpenGLVertexArray() {
        glCreateVertexArrays(1, &rendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        glDeleteVertexArrays(1, &rendererID);
    }

    void OpenGLVertexArray::addVertexBuffer(
        const Ref<VertexBuffer>& vertexBuffer) {
        glBindVertexArray(this->rendererID);
        vertexBuffer->bind();

        AE_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(),
                       "Vertex Buffer has no layout!");

        uint32_t index = 0;
        const BufferLayout& layout = vertexBuffer->getLayout();
        for (const auto& element : layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.getComponentCount(),
                                  toOpenGLBaseType(element.type),
                                  element.normalised ? GL_TRUE : GL_FALSE,
                                  layout.getStride(),
                                  (const void*)element.offset);

            index++;
        }

        vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::setIndexBuffer(
        const Ref<IndexBuffer>& indexBuffer) {
        glBindVertexArray(this->rendererID);
        indexBuffer->bind();

        this->indexBuffer = indexBuffer;
    }

    static const GLenum toOpenGLBaseType(ShaderDataType::T type) {
        switch (type) {
            case ShaderDataType::None:
                return GL_NONE;

            case ShaderDataType::Int:
                return GL_INT;
            case ShaderDataType::Int2:
                return GL_INT;
            case ShaderDataType::Int3:
                return GL_INT;
            case ShaderDataType::Int4:
                return GL_INT;

            case ShaderDataType::Float:
                return GL_FLOAT;
            case ShaderDataType::Float2:
                return GL_FLOAT;
            case ShaderDataType::Float3:
                return GL_FLOAT;
            case ShaderDataType::Float4:
                return GL_FLOAT;

            case ShaderDataType::Mat3:
                return GL_FLOAT;
            case ShaderDataType::Mat4:
                return GL_FLOAT;

            case ShaderDataType::Bool:
                return GL_BOOL;
        }

        AE_CORE_ASSERT(false, "Unknown ShaderDataType");
        return 0;
    }
};  // namespace AnEngine
