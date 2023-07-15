#include "aepch.hpp"

#include "Renderer/Shader.hpp"

#include "Exceptions/NotImplementedException.hpp"
#include "File/InputFileStream.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    Ref<Shader> Shader::create(const std::string& vertShaderPath,
                               const std::string& fragShaderPath) {
        InputFileStream vertShaderStream(vertShaderPath, std::ios::binary);
        InputFileStream fragShaderStream(fragShaderPath, std::ios::binary);

        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return std::make_shared<OpenGLShader>(vertShaderStream, fragShaderStream);

            case RenderAPI::DirectX11:
                throw NotImplementedException();
                // return std::make_shared<DX11Shader>(vertShaderStream,
                // fragShaderStream);

            case RenderAPI::DirectX12:
                throw NotImplementedException();
                // return std::make_shared<DX12Shader>(vertShaderStream,
                // fragShaderStream);

            case RenderAPI::Vulkan:
                throw NotImplementedException();
                // return std::make_shared<VulkanShader>(vertShaderStream,
                // fragShaderStream);

            default:
                AE_CORE_ASSERT(false, "Unknown RendererAPI!");
                throw NotImplementedException();
        }
    }

    Ref<Shader> Shader::create(const std::string& mixedShaderPath) {
        InputFileStream mixedShaderStream(mixedShaderPath, std::ios::binary);

        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return std::make_shared<OpenGLShader>(mixedShaderStream);

            case RenderAPI::DirectX11:
                throw NotImplementedException();
                // return std::make_shared<DX11Shader>(vertShaderStream,
                // fragShaderStream);

            case RenderAPI::DirectX12:
                throw NotImplementedException();
                // return std::make_shared<DX12Shader>(vertShaderStream,
                // fragShaderStream);

            case RenderAPI::Vulkan:
                throw NotImplementedException();
                // return std::make_shared<VulkanShader>(vertShaderStream,
                // fragShaderStream);

            default:
                AE_CORE_ASSERT(false, "Unknown RendererAPI!");
                throw NotImplementedException();
        }
    }
}  // namespace AnEngine
