#ifndef SHADERUNIFORM_HPP
#define SHADERUNIFORM_HPP

#include <glm/glm.hpp>

#include <any>
#include <string>

#include "Core/Concepts.hpp"
#include "Core/Core.hpp"


namespace AnEngine {
    struct Sampler2D {
        uint32_t slot;

        bool operator==(const Sampler2D& other) const { return slot == other.slot; }
    };

    struct ShaderUniformBase {
        std::string name;
        std::any uniform;
    };

    template <GLSLDataType UniformType>
    struct ShaderUniform : public ShaderUniformBase {
        UniformType uniform;

        ShaderUniform(const std::string& name, UniformType uniform)
            : ShaderUniformBase{name, uniform}, uniform{uniform} {}
    };

    class ShaderUniformVector : public std::vector<ShaderUniformBase> {
    public:
        using std::vector<ShaderUniformBase>::vector;

        std::any& operator[](const std::string& name) {
            for (auto& item : *this) {
                if (item.name == name) return item.uniform;
            }
            AE_CORE_ERROR("{0} couldn't be found in ShaderUniformVector", name);
        }

        template <typename T>
        T getOr(const std::string& name, T emptyType) const {
            for (auto& item : *this) {
                if (item.name == name)
                    return item.uniform.type() == typeid(T)
                               ? std::any_cast<T>(item.uniform)
                               : emptyType;
            }
            return emptyType;
        }
    };


};  // namespace AnEngine

#endif
