#include "aepch.hpp"

#include "Renderer/Shader.hpp"
#include "Renderer/Renderer.hpp"
#include "Exceptions/NotImplementedException.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"


namespace AnEngine {
    Shader* Shader::create(InputFileStream& vertShaderStream, InputFileStream& fragShaderStream) {
        switch (Renderer::getAPI()) {
        case RenderAPI::OpenGL:
            return new OpenGLShader(vertShaderStream, fragShaderStream);

        case RenderAPI::DirectX11:
            throw NotImplementedException();
            //     return new DX11Shader(vertShaderStream, fragShaderStream);

        case RenderAPI::DirectX12:
            throw NotImplementedException();
            //      return new DX12Shader(vertShaderStream, fragShaderStream);

        case RenderAPI::Vulkan:
            throw NotImplementedException();
            //     return new VulkanShader(vertShaderStream, fragShaderStream);

        default:
            AE_CORE_ASSERT(false, "Unknown RendererAPI!");
            throw NotImplementedException();
        }
    }

    Shader* Shader::create(const std::string& vertShaderSrc, const std::string& fragShaderSrc) {
        switch (Renderer::getAPI()) {
        case RenderAPI::OpenGL:
            return new OpenGLShader(vertShaderSrc, fragShaderSrc);

        case RenderAPI::DirectX11:
            throw NotImplementedException();
            //     return new DX11Shader(vertShaderSrc, fragShaderSrc);

        case RenderAPI::DirectX12:
            throw NotImplementedException();
            //      return new DX12Shader(vertShaderSrc, fragShaderSrc);

        case RenderAPI::Vulkan:
            throw NotImplementedException();
            //     return new VulkanShader(vertShaderSrc, fragShaderSrc);

        default:
            AE_CORE_ASSERT(false, "Unknown RendererAPI!");
            throw NotImplementedException();
        }
    }

}
