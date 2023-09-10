#ifndef OGLFRAMEBUFFER_HPP
#define OGLFRAMEBUFFER_HPP

#include "Core/Core.hpp"
#include "Renderer/FrameBuffer.hpp"


namespace AnEngine {
    class OpenGLFrameBuffer : public FrameBuffer {
    public:
        OpenGLFrameBuffer(const FrameBufferSpec& spec);
        virtual ~OpenGLFrameBuffer() override;

        virtual const FrameBufferSpec& getSpecification() const override {
            return specification;
        }

        virtual uint32_t getColorAttachmentID() const override {
            return colourAttachment;
        }

        virtual void reconstruct() override;
        virtual void resize(uint32_t width, uint32_t height) override;

        virtual void bind() const override;
        virtual void unBind() const override;

    private:
        FrameBufferSpec specification;
        uint32_t colourAttachment = 0;
        uint32_t depthAttachment = 0;
        RenderID rendererID = 0;
    };
}  // namespace AnEngine

#endif
