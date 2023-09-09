#ifndef OGLFRAMEBUFFER_HPP
#define OGLFRAMEBUFFER_HPP

#include "Core/Core.hpp"
#include "Renderer/FrameBuffer.hpp"


namespace AnEngine {
    class OpenGLFrameBuffer : public FrameBuffer {
    public:
        OpenGLFrameBuffer(const FrameBufferSpec& spec);
        virtual ~OpenGLFrameBuffer();

        virtual const FrameBufferSpec& getSpecification() const override {
            return specification;
        }

        virtual uint32_t getColorAttachmentID() const override {
            return colourAttachment;
        }

        virtual void reconstruct() override;

        virtual void bind() const override;
        virtual void unBind() const override;

    private:
        FrameBufferSpec specification;
        uint32_t colourAttachment, depthAttachment;
        RenderID rendererID;
    };
}  // namespace AnEngine

#endif
