#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/glm.hpp>

#include <any>
#include <vector>

#include "File/InputFileStream.hpp"


namespace AnEngine {
    class Shader {
    public:
        virtual ~Shader() = default;

        static Ref<Shader> create(const std::string& vertShaderPath,
                                  const std::string& fragShaderPath);

        static Ref<Shader> create(const std::string& mixedShaderPath);


        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void uploadUniform(const std::string& name, std::any uniform) = 0;

    protected:
        virtual uint32_t compile(
            const std::unordered_map<uint32_t, std::string>& shaderSources) const = 0;
    };
}  // namespace AnEngine

#endif
