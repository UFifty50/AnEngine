#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/glm.hpp>

#include <any>
#include <unordered_map>

#include "File/InputFileStream.hpp"


namespace AnEngine {
    class Shader {
    public:
        virtual ~Shader() = default;

        static Ref<Shader> create(const std::string& mixedShaderPath,
                                  const std::string& name = "");


        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void uploadUniform(const std::string& name, std::any uniform) = 0;

        virtual const std::string& getName() const = 0;

    protected:
        virtual uint32_t compile(
            const std::unordered_map<uint32_t, std::string>& shaderSources) const = 0;
    };

    class ShaderLibrary {
    public:
        void add(const Ref<Shader>& shader);
        void add(const std::string& name, const Ref<Shader>& shader);

        Ref<Shader> load(const std::string& mixedShaderPath);
        Ref<Shader> load(const std::string& name, const std::string& mixedShaderPath);

        Ref<Shader> get(const std::string& name);

        std::unordered_map<std::string, Ref<Shader>>::iterator begin();
        std::unordered_map<std::string, Ref<Shader>>::iterator end();

    private:
        std::unordered_map<std::string, Ref<Shader>> shaders;
    };
}  // namespace AnEngine

#endif
