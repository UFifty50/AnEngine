#ifndef OGLINDEXBUFFER_HPP
#define OGLINDEXBUFFER_HPP

#include "Renderer/Buffers/IndexBuffer.hpp"


namespace AnEngine {
    class OpenGLIndexBuffer : public IndexBuffer {
    private:
        uint32_t rendererID;
        uint32_t count;

    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual uint32_t getCount() const;

        virtual void bind() const;
        virtual void unBind() const;
    };
}

#endif
