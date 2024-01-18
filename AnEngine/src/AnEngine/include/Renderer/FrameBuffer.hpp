#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "Core/Core.hpp"
#include "glm/glm.hpp"


namespace AnEngine {
    class FrameBufferTexFormat {
    public:
        enum T : uint8_t {
            None = 0,

            // Colour
            RGBA8,
            RED_INTEGER,

            // Depth/stencil
            DEPTH24STENCIL8,

            // Defaults
            Depth = DEPTH24STENCIL8
        };

        constexpr FrameBufferTexFormat() = default;
        constexpr FrameBufferTexFormat(T format) : texFormat(format) {}

        constexpr operator T() const { return texFormat; }

        constexpr bool isDepth() const {
            switch (texFormat) {
                case T::DEPTH24STENCIL8:
                    return true;
                default:
                    return false;
            }
        }

        constexpr std::string toString() {
            switch (texFormat) {
                case T::None:
                    return "None";
                case T::RGBA8:
                    return "RGBA8";
                case T::RED_INTEGER:
                    return "RED_INTEGER";
                case T::DEPTH24STENCIL8:
                    return "DEPTH24STENCIL8";
                default:
                    return "Unknown";
            }
        }

    private:
        T texFormat = T::None;
    };

    struct FrameBufferTexSpec {
        FrameBufferTexFormat texFormat = FrameBufferTexFormat::None;

        FrameBufferTexSpec() = default;
        FrameBufferTexSpec(FrameBufferTexFormat::T format) : texFormat(format) {}
    };

    struct FrameBufferAttachmentSpec {
        std::vector<FrameBufferTexSpec> Attachments;

        FrameBufferAttachmentSpec() = default;
        FrameBufferAttachmentSpec(std::initializer_list<FrameBufferTexSpec> attachments)
            : Attachments(attachments) {}
    };

    struct FrameBufferSpec {
        uint32_t Width, Height;
        FrameBufferAttachmentSpec Attachments;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class FrameBuffer {
    public:
        virtual ~FrameBuffer() = default;

        virtual const FrameBufferSpec& getSpecification() const = 0;
        virtual uint32_t getColorAttachmentID(uint32_t index = 0) const = 0;
        virtual std::vector<int32_t> readPixels(uint32_t attachmentIndex, glm::vec2 from,
                                                glm::vec2 size,
                                                FrameBufferTexFormat format) const = 0;

        virtual void clearColourAttachment(uint32_t attachmentIndex, int32_t value) = 0;

        virtual void reconstruct() = 0;
        virtual void resize(uint32_t width, uint32_t height) = 0;

        virtual void bind() const = 0;
        virtual void unBind() const = 0;


        static Ref<FrameBuffer> create(const FrameBufferSpec& spec);
    };
}  // namespace AnEngine

#endif
