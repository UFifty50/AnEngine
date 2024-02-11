#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstdint>
#include <filesystem>
#include <string>

#include "Renderer/ShaderUniform.hpp"


namespace fs = std::filesystem;

namespace AnEngine {
    class ImageFormat {
    public:
        enum PixelFormat {
            None = 0,
            RGB = 3,
            RGBA = 4,
        };

        enum FileFormat {
            JPG = 0,
            PNG = 1,
            BMP = 2,
            TGA = 3,
            DDS = 4,
        };

        static uint8_t getChannelCount(PixelFormat format) {
            switch (format) {
                case RGB:
                    return 3;
                case RGBA:
                    return 4;
            }
            return 0;
        }

        static uint8_t getBPP(PixelFormat format) { return getChannelCount(format); }

        static std::string toString(PixelFormat format) {
            switch (format) {
                case RGB:
                    return "RGB";
                case RGBA:
                    return "RGBA";
            }
            return "None";
        }

        static std::string toString(FileFormat format) {
            switch (format) {
                case JPG:
                    return "JPG";
                case PNG:
                    return "PNG";
                case BMP:
                    return "BMP";
                case TGA:
                    return "TGA";
                case DDS:
                    return "DDS";
            }
            return "None";
        }
    };

    class Texture {
    public:
        virtual ~Texture() = default;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;

        virtual ImageFormat::FileFormat getFileFormat() const = 0;
        virtual ImageFormat::PixelFormat getPixelFormat() const = 0;

        virtual Sampler2D getSampler() const = 0;
        virtual void setData(void* data, uint32_t size) = 0;

        virtual fs::path getPath() const = 0;

        //     virtual Ref<Texture> getSubImage(glm::vec2 coords, glm::vec2 size) const =
        //     0;

        virtual void bind(uint32_t slot = 0) const = 0;

        virtual bool operator==(const Texture& other) const = 0;
    };
    ;
};  // namespace AnEngine

#endif
