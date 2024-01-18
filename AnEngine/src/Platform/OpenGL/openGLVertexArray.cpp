#include "aepch.hpp"

#include "Platform/OpenGL/openGLVertexArray.hpp"

#include <glad/glad.h>

#include "Renderer/Buffers/BufferLayout.hpp"
#include "Renderer/Buffers/IndexBuffer.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"


namespace AnEngine {
    OpenGLVertexArray::OpenGLVertexArray() { glCreateVertexArrays(1, &rendererID); }

    OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &rendererID); }

    void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
        glBindVertexArray(this->rendererID);
        vertexBuffer->bind();

        AE_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(),
                       "Vertex Buffer has no layout!");

        uint32_t index = 0;
        const BufferLayout& layout = vertexBuffer->getLayout();
        for (const auto& element : layout) {
            glEnableVertexAttribArray(index);

            if (element.type < (ShaderDataType)6) {
                glVertexAttribIPointer(index, element.getComponentCount(),
                                       toOpenGLBaseType(element.type), layout.getStride(),
                                       (const void*)element.offset);
            } else {
                glVertexAttribPointer(index, element.getComponentCount(),
                                      toOpenGLBaseType(element.type),
                                      element.normalised ? GL_TRUE : GL_FALSE,
                                      layout.getStride(), (const void*)element.offset);
            }
            index++;
        }

        vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
        glBindVertexArray(this->rendererID);
        indexBuffer->bind();

        this->indexBuffer = indexBuffer;
    }

    static const GLenum toOpenGLBaseType(ShaderDataType::T type) {
        // clang-format off
        switch (type) {
        case ShaderDataType::None:    return GL_NONE;

        case ShaderDataType::Bool:
        case ShaderDataType::Int: 
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:    return GL_INT;

        case ShaderDataType::Float: 
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:  return GL_FLOAT;

        case ShaderDataType::Mat3:  
        case ShaderDataType::Mat4:    return GL_FLOAT;
        }
        // clang-format on

        AE_CORE_ASSERT(false, "Unknown ShaderDataType");
        return 0;
    }
};  // namespace AnEngine
