#include "aepch.hpp"

#include "Platform/OpenGL/Texture/OpenGLTexture2D.hpp"

#include <glad/glad.h>

#include <string>

#include "stb_image.h"


namespace AnEngine {
    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        : file(InputFileStream(path)) {
        // TODO: asset build pipeline - custom file format
        int width, height, channels, internalFormat, dataFormat;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

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

        glTextureSubImage2D(this->rendererID, 0, 0, 0, this->width, this->height,
                            dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &this->rendererID); }

    void OpenGLTexture2D::bind(uint32_t slot) const {
        glBindTextureUnit(slot, this->rendererID);
    }
};  // namespace AnEngine
