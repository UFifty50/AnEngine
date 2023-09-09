#ifndef OPENGLTEXTURE2D_HPP
#define OPENGLTEXTURE2D_HPP

#include <glad/glad.h>

#include "File/InputFileStream.hpp"
#include "Renderer/ShaderUniform.hpp"
#include "Texture/Texture2D.hpp"


namespace AnEngine {
    class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(const std::string& path);
        OpenGLTexture2D(uint32_t width, uint32_t height);
        virtual ~OpenGLTexture2D();

        inline virtual uint32_t getWidth() const override { return width; }
        inline virtual uint32_t getHeight() const override { return height; }

        inline virtual ImageFormat::FileFormat getFileFormat() const override {
            return fileFormat;
        }

        inline virtual ImageFormat::PixelFormat getPixelFormat() const override {
            return pixelFormat;
        }

        inline virtual Sampler2D getSampler() const override {
            return Sampler2D{rendererID};
        }
        virtual void setData(void* data, uint32_t size) override;

        virtual void bind(uint32_t slot) const override;

        virtual bool operator==(const Texture& other) const override {
            return rendererID == ((OpenGLTexture2D&)other).rendererID;
        }

        Ref<Texture> getSubImage(glm::vec2 coords, glm::vec2 size) const;

    private:
        InputFileStream file;

        uint32_t width;
        uint32_t height;

        RenderID rendererID;

        ImageFormat::FileFormat fileFormat;
        ImageFormat::PixelFormat pixelFormat;

        uint8_t* imageData;
    };
};  // namespace AnEngine

#endif
