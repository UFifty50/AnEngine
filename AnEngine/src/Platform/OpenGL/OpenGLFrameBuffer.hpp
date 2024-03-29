#ifndef OGLFRAMEBUFFER_HPP
#define OGLFRAMEBUFFER_HPP

#include "Core/Core.hpp"
#include "Renderer/FrameBuffer.hpp"
#include "glm/glm.hpp"


namespace AnEngine {
    class OpenGLFrameBuffer : public FrameBuffer {
    public:
        OpenGLFrameBuffer(const FrameBufferSpec& spec);
        virtual ~OpenGLFrameBuffer() override;

        virtual const FrameBufferSpec& getSpecification() const override {
            return specification;
        }

        virtual uint32_t getColorAttachmentID(uint32_t index = 0) const override {
            AE_CORE_ASSERT(index < colourAttachments.size(), "Index out of range!");
            return colourAttachments[index];
        }

        virtual std::vector<int32_t> readPixels(uint32_t attachmentIndex, glm::vec2 from,
                                                glm::vec2 size,
                                                FrameBufferTexFormat format) const override;

        virtual void clearColourAttachment(uint32_t attachmentIndex, int32_t value) override;

        virtual void reconstruct() override;
        virtual void resize(uint32_t width, uint32_t height) override;

        virtual void bind() const override;
        virtual void unBind() const override;

    private:
        FrameBufferSpec specification;
        std::vector<RenderID> colourAttachments;
        RenderID depthAttachment = 0;
        RenderID rendererID = 0;

        std::vector<FrameBufferTexSpec> colourAttachmentSpecs;
        FrameBufferTexSpec depthAttachmentSpec = FrameBufferTexFormat::None;
    };
}  // namespace AnEngine

#endif
