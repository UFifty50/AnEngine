#ifndef OPENGLTEXTURE2D_HPP
#define OPENGLTEXTURE2D_HPP

#include <glad/glad.h>

#include "File/InputFileStream.hpp"
#include "Texture/Texture2D.hpp"


namespace AnEngine {
    class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(const std::string& path);
        virtual ~OpenGLTexture2D();

        inline virtual uint32_t getWidth() const override { return width; }
        inline virtual uint32_t getHeight() const override { return height; }

        inline virtual ImageFormat::FileFormat getFileFormat() const override {
            return fileFormat;
        }

        inline virtual ImageFormat::PixelFormat getPixelFormat() const override {
            return pixelFormat;
        }

        virtual void bind(uint32_t slot) const override;

    private:
        InputFileStream file;

        uint32_t width;
        uint32_t height;

        uint32_t rendererID;

        ImageFormat::FileFormat fileFormat;
        ImageFormat::PixelFormat pixelFormat;
    };
};  // namespace AnEngine

#endif
