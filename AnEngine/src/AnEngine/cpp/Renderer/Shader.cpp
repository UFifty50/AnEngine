#include "aepch.hpp"

#include "Renderer/Shader.hpp"

#include "Exceptions/NotImplementedException.hpp"
#include "File/InputFileStream.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    Ref<Shader> Shader::create(const std::string& mixedShaderPath, const std::string& name) {
        AE_PROFILE_FUNCTION()

        InputFileStream mixedShaderStream(mixedShaderPath, std::ios::binary);

        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return MakeRef<OpenGLShader>(mixedShaderStream, name);

            case RenderAPI::DirectX11:
                throw NotImplementedException();
                // return MakeRef<DX11Shader>(vertShaderStream,
                // fragShaderStream);

            case RenderAPI::DirectX12:
                throw NotImplementedException();
                // return MakeRef<DX12Shader>(vertShaderStream,
                // fragShaderStream);

            case RenderAPI::Vulkan:
                throw NotImplementedException();
                // return MakeRef<VulkanShader>(vertShaderStream,
                // fragShaderStream);

            default:
                AE_CORE_ASSERT(false, "Unknown RendererAPI!");
                throw NotImplementedException();
        }
    }


    void ShaderLibrary::add(const Ref<Shader>& shader) {
        AE_PROFILE_FUNCTION()

        auto& name = shader->getName();
        add(name, shader);
    }

    void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader) {
        AE_PROFILE_FUNCTION()

        AE_CORE_ASSERT(shaders.find(name) == shaders.end(), "Shader already exists!");
        shaders[name] = shader;
    }

    Ref<Shader> ShaderLibrary::load(const std::string& mixedShaderPath) {
        AE_PROFILE_FUNCTION()

        auto shader = Shader::create(mixedShaderPath);
        add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::load(const std::string& name,
                                    const std::string& mixedShaderPath) {
        AE_PROFILE_FUNCTION()

        if (shaders.find(name) != shaders.end()) return shaders[name];

        auto shader = Shader::create(mixedShaderPath);
        add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::get(const std::string& name) {
        AE_PROFILE_FUNCTION()

        AE_CORE_ASSERT(shaders.find(name) != shaders.end(), "Shader not found!");
        return shaders[name];
    }

    std::unordered_map<std::string, Ref<Shader>>::iterator ShaderLibrary::begin() {
        AE_PROFILE_FUNCTION()

        return shaders.begin();
    }

    std::unordered_map<std::string, Ref<Shader>>::iterator ShaderLibrary::end() {
        AE_PROFILE_FUNCTION()

        return shaders.end();
    }
}  // namespace AnEngine
