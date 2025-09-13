#ifndef OPENGLTEXTURE2D_HPP
#define OPENGLTEXTURE2D_HPP

#include <glad/glad.h>

#include <filesystem>

#include "File/FileStream.hpp"
#include "Renderer/ShaderUniform.hpp"
#include "Texture/Texture2D.hpp"


namespace fs = std::filesystem;

namespace AnEngine {
class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(const fs::path& path);
    OpenGLTexture2D(uint32_t width, uint32_t height);
    ~OpenGLTexture2D() override;

    uint32_t getWidth() const override { return width; }
    uint32_t getHeight() const override { return height; }

    ImageFormat::FileFormat getFileFormat() const override { return fileFormat; }

    ImageFormat::PixelFormat getPixelFormat() const override { return pixelFormat; }

    Sampler2D getSampler() const override { return Sampler2D{rendererID}; }
    void setData(void* data, uint32_t size) override;

    void bind(uint32_t slot) const override;

    bool operator==(const Texture& other) const override {
        return rendererID == ((OpenGLTexture2D&)other).rendererID;
    }

    fs::path getPath() const override { return file.getFilePath(); }

    //  Ref<Texture> getSubImage(glm::vec2 coords, glm::vec2 size) const;

private:
    FileStreamReader file;

    uint32_t width;
    uint32_t height;

    RenderID rendererID;

    ImageFormat::FileFormat fileFormat;
    ImageFormat::PixelFormat pixelFormat;

    uint8_t* imageData;
};
}; // namespace AnEngine

#endif
