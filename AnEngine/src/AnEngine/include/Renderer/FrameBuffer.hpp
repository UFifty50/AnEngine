#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "Core/Core.hpp"


namespace AnEngine {
    struct FrameBufferSpec {
        uint32_t Width, Height;
        // TODO: format
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class FrameBuffer {
    public:
        virtual ~FrameBuffer() = default;

        virtual const FrameBufferSpec& getSpecification() const = 0;
        virtual uint32_t getColorAttachmentID() const = 0;
        virtual void reconstruct() = 0;

        virtual void bind() const = 0;
        virtual void unBind() const = 0;


        static Ref<FrameBuffer> create(const FrameBufferSpec& spec);
    };
}  // namespace AnEngine

#endif
