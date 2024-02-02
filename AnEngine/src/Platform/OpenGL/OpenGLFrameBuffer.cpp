#include "aepch.hpp"

#include "Platform/OpenGL/OpenGLFrameBuffer.hpp"

#include <glad/glad.h>


namespace AnEngine {
    static constexpr uint32_t MaxFrameBufferSize = 8192;  // replace with GPU capabilities

    namespace Utils {
        static GLenum TextureTarget(bool isMultisampled) {
            AE_PROFILE_FUNCTION()

            return isMultisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void CreateTextures(bool isMultisampled, RenderID* outID, uint32_t count) {
            AE_PROFILE_FUNCTION()

            glCreateTextures(TextureTarget(isMultisampled), count, outID);
        }

        static void BindTexture(bool isMultisampled, RenderID id) {
            AE_PROFILE_FUNCTION()

            glBindTexture(TextureTarget(isMultisampled), id);
        }

        static void AttachColourTexture(RenderID id, uint32_t samples, GLenum glFormat,
                                        GLenum format, uint32_t width, uint32_t height,
                                        uint32_t index) {
            AE_PROFILE_FUNCTION()

            bool isMultisampled = samples > 1;

            if (isMultisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, glFormat, width,
                                        height, GL_FALSE);
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, format,
                             GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                // TODO: filtering
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
                                   TextureTarget(isMultisampled), id, 0);
        }

        static void AttachDepthTexture(RenderID id, uint32_t samples, GLenum format,
                                       GLenum type, uint32_t width, uint32_t height) {
            AE_PROFILE_FUNCTION()

            bool isMultisampled = samples > 1;

            if (isMultisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width,
                                        height, GL_FALSE);
            } else {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, type, TextureTarget(isMultisampled), id, 0);
        }

        static GLenum GLformat(FrameBufferTexFormat format) {
            AE_PROFILE_FUNCTION()

            switch (format) {
                case FrameBufferTexFormat::RGBA8:
                    return GL_RGBA;
                case FrameBufferTexFormat::RED_INTEGER:
                    return GL_RED_INTEGER;
                default:
                    AE_CORE_ASSERT(false, "Unknown or unsupported format!");
                    return 0;
            }
        }

        static GLenum GLformatType(FrameBufferTexFormat format) {
            AE_PROFILE_FUNCTION()

            switch (format) {
                case FrameBufferTexFormat::RGBA8:
                    return GL_UNSIGNED_BYTE;
                case FrameBufferTexFormat::RED_INTEGER:
                    return GL_INT;
                default:
                    AE_CORE_ASSERT(false, "Unknown or unsupported format!");
                    return 0;
            }
        }

        // return size of the format in bytes
        static uint32_t GLsizeOf(FrameBufferTexFormat format) {
            AE_PROFILE_FUNCTION()

            switch (format) {
                case FrameBufferTexFormat::RGBA8:
                    return 4;
                case FrameBufferTexFormat::RED_INTEGER:
                    return 1;
                default:
                    AE_CORE_ASSERT(false, "Unknown or unsupported format!");
                    return 0;
            }
        }

    }  // namespace Utils

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpec& spec) : specification(spec) {
        AE_PROFILE_FUNCTION()

        for (const auto& attachmentSpec : specification.Attachments.Attachments) {
            if (attachmentSpec.texFormat.isDepth())
                depthAttachmentSpec = attachmentSpec;
            else
                colourAttachmentSpecs.emplace_back(attachmentSpec);
        }

        reconstruct();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer() {
        AE_PROFILE_FUNCTION()

        glDeleteFramebuffers(1, &rendererID);
        glDeleteTextures((GLsizei)colourAttachments.size(), colourAttachments.data());
        glDeleteTextures(1, &depthAttachment);
    }

    std::vector<int32_t> OpenGLFrameBuffer::readPixels(uint32_t attachmentIndex,
                                                       glm::vec2 from, glm::vec2 size,
                                                       FrameBufferTexFormat format) const {
        AE_PROFILE_FUNCTION()

        // clang-format off
        AE_CORE_ASSERT(attachmentIndex < colourAttachments.size(), "Index out of range!");
        AE_CORE_ASSERT(from.x >= 0 && from.x <= (int)specification.Width, "from.x out of range!");
        AE_CORE_ASSERT(from.y >= 0 && from.y <= (int)specification.Height, "from.y out of range!");
        // clang-format on

        if (from.x + size.x > specification.Width + 1) size.x = specification.Width - from.x;
        if (from.y + size.y > specification.Height + 1) size.y = specification.Height - from.y;

        std::vector<int32_t> pixels(size.x * size.y * Utils::GLsizeOf(format));

        //   glBindFramebuffer(GL_READ_FRAMEBUFFER, rendererID);
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        glReadPixels((GLsizei)from.x, (GLsizei)from.y, (GLsizei)size.x, (GLsizei)size.y,
                     Utils::GLformat(format), GL_INT, pixels.data());
        //    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        return pixels;
    }


    void OpenGLFrameBuffer::clearColourAttachment(uint32_t attachmentIndex, int32_t value) {
        AE_PROFILE_FUNCTION()

        AE_CORE_ASSERT(attachmentIndex < colourAttachments.size(), "Index out of range!");

        auto& spec = colourAttachmentSpecs[attachmentIndex];
        glClearTexImage(colourAttachments[attachmentIndex], 0, Utils::GLformat(spec.texFormat),
                        GL_INT, &value);
    }

    void OpenGLFrameBuffer::reconstruct() {
        AE_PROFILE_FUNCTION()

        if (rendererID != 0) {
            glDeleteFramebuffers(1, &rendererID);
            glDeleteTextures((GLsizei)colourAttachments.size(), colourAttachments.data());
            glDeleteTextures(1, &depthAttachment);

            colourAttachments.clear();
            depthAttachment = 0;
        }

        glCreateFramebuffers(1, &rendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

        bool isMultisampled = specification.Samples > 1;


        // Attachments
        if (colourAttachmentSpecs.size() > 0) {
            colourAttachments.resize(colourAttachmentSpecs.size());

            Utils::CreateTextures(isMultisampled, colourAttachments.data(),
                                  (uint32_t)colourAttachments.size());

            for (uint32_t i = 0; i < colourAttachments.size(); i++) {
                Utils::BindTexture(isMultisampled, colourAttachments[i]);

                switch (colourAttachmentSpecs[i].texFormat) {
                    case FrameBufferTexFormat::RGBA8: {
                        Utils::AttachColourTexture(colourAttachments[i], specification.Samples,
                                                   GL_RGBA8, GL_RGBA, specification.Width,
                                                   specification.Height, i);
                        break;
                    }

                    case FrameBufferTexFormat::RED_INTEGER: {
                        Utils::AttachColourTexture(
                            colourAttachments[i], specification.Samples, GL_R32I,
                            GL_RED_INTEGER, specification.Width, specification.Height, i);
                        break;
                    }

                    default: {
                        AE_CORE_ASSERT(false,
                                       "Unknown or unsupported colour texture format {}!",
                                       colourAttachmentSpecs[i].texFormat.toString());
                    }
                }
            }
        }

        if (depthAttachmentSpec.texFormat != FrameBufferTexFormat::None) {
            Utils::CreateTextures(isMultisampled, &depthAttachment, 1);
            Utils::BindTexture(isMultisampled, depthAttachment);

            switch (depthAttachmentSpec.texFormat) {
                case FrameBufferTexFormat::DEPTH24STENCIL8: {
                    Utils::AttachDepthTexture(depthAttachment, specification.Samples,
                                              GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
                                              specification.Width, specification.Height);
                    break;
                }

                default: {
                    AE_CORE_ASSERT(false, "Unknown or unsupported depth texture format {}!",
                                   depthAttachmentSpec.texFormat.toString());
                }
            }
        }

        if (colourAttachments.size() > 1) {
            AE_CORE_ASSERT(colourAttachmentSpecs.size() <= 32, "Too many attachments!");
            GLenum buffers[32] = {
                GL_COLOR_ATTACHMENT0,  GL_COLOR_ATTACHMENT1,  GL_COLOR_ATTACHMENT2,
                GL_COLOR_ATTACHMENT3,  GL_COLOR_ATTACHMENT4,  GL_COLOR_ATTACHMENT5,
                GL_COLOR_ATTACHMENT6,  GL_COLOR_ATTACHMENT7,  GL_COLOR_ATTACHMENT8,
                GL_COLOR_ATTACHMENT9,  GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
                GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14,
                GL_COLOR_ATTACHMENT15, GL_COLOR_ATTACHMENT16, GL_COLOR_ATTACHMENT17,
                GL_COLOR_ATTACHMENT18, GL_COLOR_ATTACHMENT19, GL_COLOR_ATTACHMENT20,
                GL_COLOR_ATTACHMENT21, GL_COLOR_ATTACHMENT22, GL_COLOR_ATTACHMENT23,
                GL_COLOR_ATTACHMENT24, GL_COLOR_ATTACHMENT25, GL_COLOR_ATTACHMENT26,
                GL_COLOR_ATTACHMENT27, GL_COLOR_ATTACHMENT28, GL_COLOR_ATTACHMENT29,
                GL_COLOR_ATTACHMENT30, GL_COLOR_ATTACHMENT31};

            glDrawBuffers((GLsizei)colourAttachments.size(), buffers);
        } else if (colourAttachments.empty()) {
            // Only depth buffer
            glDrawBuffer(GL_NONE);
        }

        AE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                       "Framebuffer is incomplete!")

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::resize(uint32_t width, uint32_t height) {
        AE_PROFILE_FUNCTION()

        if ((width <= 0 || height <= 0) ||
            (width >= MaxFrameBufferSize || height >= MaxFrameBufferSize)) {
            AE_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
            return;
        }

        specification.Width = width;
        specification.Height = height;
        reconstruct();
    }

    void OpenGLFrameBuffer::bind() const {
        AE_PROFILE_FUNCTION()

        glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
        glViewport(0, 0, specification.Width, specification.Height);
    }

    void OpenGLFrameBuffer::unBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
}  // namespace AnEngine
