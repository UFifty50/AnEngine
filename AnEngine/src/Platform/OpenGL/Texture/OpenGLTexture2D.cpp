#include "aepch.hpp"

#include "Platform/OpenGL/Texture/OpenGLTexture2D.hpp"

#include <glad/glad.h>

#include <string>

#include "stb_image.h"


namespace AnEngine {
    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        : file(InputFileStream(path, std::ios::binary)) {
        AE_PROFILE_FUNCTION()

        // TODO: asset build pipeline - custom file format
        int width, height, channels, internalFormat, dataFormat;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data = nullptr;
        {
            AE_PROFILE_SCOPE(
                "OpenGLTexture2D::OpenGLTexture2D(const std::string&)@stbi_load")

            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }

        AE_CORE_ASSERT(data, "Failed to load image!");

        switch (channels) {
            case 3:
                this->pixelFormat = ImageFormat::RGB;
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
                break;

            case 4:
                this->pixelFormat = ImageFormat::RGBA;
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
                break;

            default:
                this->pixelFormat = ImageFormat::None;
                internalFormat = 0;
                dataFormat = 0;
                AE_CORE_ASSERT(false, "Unknown image format! number of channels: {1}",
                               channels);
                break;
        }


        this->width = width;
        this->height = height;

        glCreateTextures(GL_TEXTURE_2D, 1, &this->rendererID);
        glTextureStorage2D(this->rendererID, 1, internalFormat, this->width,
                           this->height);

        glTextureParameteri(this->rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(this->rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(this->rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(this->rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(this->rendererID, 0, 0, 0, this->width, this->height,
                            dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
        : width(width), height(height), file(nullptr) {
        AE_PROFILE_FUNCTION()

        this->pixelFormat = ImageFormat::RGBA;


        glCreateTextures(GL_TEXTURE_2D, 1, &this->rendererID);
        glTextureStorage2D(this->rendererID, 1, GL_RGBA8, this->width, this->height);

        glTextureParameteri(this->rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(this->rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(this->rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(this->rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &this->rendererID); }

    void OpenGLTexture2D::setData(void* data, uint32_t size) {
        AE_CORE_ASSERT(
            size == this->width * this->height * ImageFormat::getBPP(pixelFormat),
            "Data must be entire texture!");

        GLenum dataFormat;
        switch (pixelFormat) {
            case ImageFormat::RGB:
                dataFormat = GL_RGB;
                break;
            case ImageFormat::RGBA:
                dataFormat = GL_RGBA;
                break;
            default:
                AE_CORE_ASSERT(false, "Unknown image format!");
                break;
        }

        glTextureSubImage2D(this->rendererID, 0, 0, 0, this->width, this->height,
                            dataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::bind(uint32_t slot) const {
        AE_PROFILE_FUNCTION()

        glBindTextureUnit(slot, this->rendererID);
    }
};  // namespace AnEngine
