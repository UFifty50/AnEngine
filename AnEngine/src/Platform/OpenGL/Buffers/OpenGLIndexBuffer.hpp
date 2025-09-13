#ifndef OGLINDEXBUFFER_HPP
#define OGLINDEXBUFFER_HPP

#include "Core/Core.hpp"
#include "Renderer/Buffers/IndexBuffer.hpp"


namespace AnEngine {
class OpenGLIndexBuffer : public IndexBuffer {
    RenderID rendererID;
    uint32_t count;

public:
    OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
    ~OpenGLIndexBuffer() override;

    uint32_t getCount() const override;

    void bind() const override;
    void unBind() const override;
};
} // namespace AnEngine

#endif
