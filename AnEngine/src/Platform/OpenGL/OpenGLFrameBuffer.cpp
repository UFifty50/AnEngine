#include "aepch.hpp"

#include "Platform/OpenGL/OpenGLFrameBuffer.hpp"

#include <glad/glad.h>


namespace AnEngine {
    static constexpr uint32_t MaxFrameBufferSize = 8192;  // replace with GPU capabilities

    namespace Utils {
        static GLenum TextureTarget(bool isMultisampled) {
            return isMultisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void CreateTextures(bool isMultisampled,
                                   std::vector<RenderID>& colourAttachments) {
            glCreateTextures(TextureTarget(isMultisampled), colourAttachments.size(),
                             colourAttachments.data());
        }

        static void CreateTextures(bool isMultisampled, RenderID* depthAttachment) {
            glCreateTextures(TextureTarget(isMultisampled), 1, depthAttachment);
        }

        static void BindTexture(bool isMultisampled, RenderID id) {
            glBindTexture(TextureTarget(isMultisampled), id);
        }

        static void AttachColourTexture(RenderID id, uint32_t samples, GLenum format,
                                        uint32_t width, uint32_t height, uint32_t index) {
            bool isMultisampled = samples > 1;

            if (isMultisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width,
                                        height, GL_FALSE);
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA,
                             GL_UNSIGNED_BYTE, nullptr);

                glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                // TODO: filtering
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
                                   TextureTarget(isMultisampled), id, 0);
        }

        static void AttachDepthTexture(RenderID id, uint32_t samples, GLenum format,
                                       GLenum type, uint32_t width, uint32_t height) {
            bool isMultisampled = samples > 1;

            if (isMultisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width,
                                        height, GL_FALSE);
            } else {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, type, TextureTarget(isMultisampled),
                                   id, 0);
        }

    }  // namespace Utils

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpec& spec)
        : specification(spec) {
        for (const auto& spec : specification.Attachments.Attachments) {
            if (spec.texFormat.isDepth())
                depthAttachmentSpec = spec;
            else
                colourAttachmentSpecs.emplace_back(spec);
        }

        reconstruct();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer() {
        glDeleteFramebuffers(1, &rendererID);
        glDeleteTextures(colourAttachments.size(), colourAttachments.data());
        glDeleteTextures(1, &depthAttachment);
    }

    void OpenGLFrameBuffer::reconstruct() {
        if (rendererID != 0) {
            glDeleteFramebuffers(1, &rendererID);
            glDeleteTextures(colourAttachments.size(), colourAttachments.data());
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

            Utils::CreateTextures(isMultisampled, colourAttachments);

            for (uint32_t i = 0; i < colourAttachments.size(); i++) {
                Utils::BindTexture(isMultisampled, colourAttachments[i]);

                switch (colourAttachmentSpecs[i].texFormat) {
                    case FrameBufferTexFormat::RGBA8: {
                        Utils::AttachColourTexture(
                            colourAttachments[i], specification.Samples, GL_RGBA8,
                            specification.Width, specification.Height, i);
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
            Utils::CreateTextures(isMultisampled, &depthAttachment);
            Utils::BindTexture(isMultisampled, depthAttachment);

            switch (depthAttachmentSpec.texFormat) {
                case FrameBufferTexFormat::DEPTH24STENCIL8: {
                    Utils::AttachDepthTexture(depthAttachment, specification.Samples,
                                              GL_DEPTH24_STENCIL8,
                                              GL_DEPTH_STENCIL_ATTACHMENT,
                                              specification.Width, specification.Height);
                    break;
                }

                default: {
                    AE_CORE_ASSERT(false,
                                   "Unknown or unsupported depth texture format {}!",
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

            glDrawBuffers(colourAttachments.size(), buffers);
        } else if (colourAttachments.empty()) {
            // Only depth buffer
            glDrawBuffer(GL_NONE);
        }

        AE_CORE_ASSERT(
            glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
            "Framebuffer is incomplete!")

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::resize(uint32_t width, uint32_t height) {
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
        glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
        glViewport(0, 0, specification.Width, specification.Height);
    }

    void OpenGLFrameBuffer::unBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
}  // namespace AnEngine
