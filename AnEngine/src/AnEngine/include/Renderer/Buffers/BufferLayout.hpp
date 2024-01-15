#ifndef BUFFERLAYOUT_HPP
#define BUFFERLAYOUT_HPP

#include <string>
#include <vector>

#include "Core/Core.hpp"


// clang-format off
namespace AnEngine {
    class ShaderDataType {
    public:
        enum T : uint8_t {
            None = 0,

            // Vector types (int/float)
            Int, Int2, Int3, Int4,
            Float, Float2, Float3, Float4,

            // Vec defaults to Float
            Vec = Float,
            Vec2 = Float2,
            Vec3 = Float3,
            Vec4 = Float4,

            // Matrix types (always float)
            Mat3, Mat4,

            Bool
        };

        ShaderDataType() = default;
        constexpr ShaderDataType(T other) : type(other) {}

        constexpr operator T() const { return type; }

        explicit constexpr operator bool() const = delete;

        uint32_t getSize() {
            switch (type) {
                case ShaderDataType::None:    return 0;

                case ShaderDataType::Int:      return 4;
                case ShaderDataType::Int2:     return 4 * 2;
                case ShaderDataType::Int3:     return 4 * 3;
                case ShaderDataType::Int4:     return 4 * 4;

                case ShaderDataType::Float:    return 4;
                case ShaderDataType::Float2:   return 4 * 2;
                case ShaderDataType::Float3:   return 4 * 3;
                case ShaderDataType::Float4:   return 4 * 4;

                case ShaderDataType::Mat3:     return 4 * 3 * 3;
                case ShaderDataType::Mat4:     return 4 * 4 * 4;

                case ShaderDataType::Bool:     return 1;
            }

            AE_CORE_ASSERT(false, "Unknown ShaderDataType");
            return 0;
        }

        private:
            ShaderDataType::T type;
    };

    struct BufferElement {
        std::string name;
        ShaderDataType type;
        uint32_t size;
        uint32_t offset;
        bool normalised;

        BufferElement() = default;
        BufferElement(ShaderDataType type, std::string const& name, bool normalised = false)
            : name(name), type(type), size(type.getSize()), offset(0), normalised(normalised) {}

        uint32_t getComponentCount() const {
            switch (type) {
                case ShaderDataType::None:    return 0;

                case ShaderDataType::Int:
                case ShaderDataType::Float:   return 1;

                case ShaderDataType::Int2: 
                case ShaderDataType::Float2:  return 2;

                case ShaderDataType::Int3: 
                case ShaderDataType::Float3:  return 3;

                case ShaderDataType::Int4: 
                case ShaderDataType::Float4:  return 4;

                case ShaderDataType::Mat3:    return 3 * 3;
                case ShaderDataType::Mat4:    return 4 * 4;

                case ShaderDataType::Bool:    return 1;
            }

            AE_CORE_ASSERT(false, "Unknown ShaderDataType");
            return 0;
        }
    };

    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> const& elements)
            : elements(elements) {
            calculateOffsetAndStride();
        }

        inline const std::vector<BufferElement>& getElements() const { return elements; }
        inline const uint32_t getStride() const { return stride; }

        std::vector<BufferElement>::iterator begin() { return elements.begin(); }
        std::vector<BufferElement>::iterator end() { return elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return elements.end(); }
    
    private:
        std::vector<BufferElement> elements;
        uint32_t stride = 0;

        void calculateOffsetAndStride() {
            uint32_t offset = 0;
            stride = 0;
            for (auto& element : elements) {
                element.offset = offset;
                offset += element.size;
                stride += element.size;
            }
        }
    };
}

#endif
