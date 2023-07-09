#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstdint>
#include <string>


namespace AnEngine {
    class ImageFormat {
    public:
        enum PixelFormat {
            None = 0,
            RGB = 1,
            RGBA = 2,
        };

        enum FileFormat {
            JPG = 0,
            PNG = 1,
            BMP = 2,
            TGA = 3,
            DDS = 4,
        };

        static uint32_t getChannelCount(PixelFormat format) {
            switch (format) {
                case RGB:
                    return 3;
                case RGBA:
                    return 4;
            }
            return 0;
        }

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

        virtual void bind() const = 0;
    };
    ;
};  // namespace AnEngine

#endif
