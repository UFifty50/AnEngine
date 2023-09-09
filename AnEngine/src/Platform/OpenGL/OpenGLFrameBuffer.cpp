#include "aepch.hpp"

#include "Platform/OpenGL/OpenGLFrameBuffer.hpp"

#include <glad/glad.h>


namespace AnEngine {
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpec& spec)
        : specification(spec) {
        reconstruct();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer() { glDeleteFramebuffers(1, &rendererID); }

    void OpenGLFrameBuffer::reconstruct() {
        glCreateFramebuffers(1, &rendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

        glCreateTextures(GL_TEXTURE_2D, 1, &colourAttachment);
        glBindTexture(GL_TEXTURE_2D, colourAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification.Width,
                     specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTextureParameteri(colourAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(colourAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                               colourAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
        glBindTexture(GL_TEXTURE_2D, depthAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, specification.Width,
                     specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8,
                     nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                               depthAttachment, 0);

        AE_CORE_ASSERT(
            glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
            "Framebuffer is incomplete!")

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
        glViewport(0, 0, specification.Width, specification.Height);
    }

    void OpenGLFrameBuffer::unBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
}  // namespace AnEngine
