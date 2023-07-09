#ifndef OPENGLTEXTURE2D_HPP
#define OPENGLTEXTURE2D_HPP

#include <glad/glad.h>

#include "Texture/Texture2D.hpp"


namespace AnEngine {
    class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(const std::string& data);

        virtual uint32_t getWidth() const;
        virtual uint32_t getHeight() const;

        virtual ImageFormat::FileFormat getFileFormat() const;
        virtual ImageFormat::PixelFormat getPixelFormat() const;

        virtual void bind() const;
    };
};  // namespace AnEngine

#endif
