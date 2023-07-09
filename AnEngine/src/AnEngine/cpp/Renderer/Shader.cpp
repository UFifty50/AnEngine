#include "aepch.hpp"

#include "Renderer/Shader.hpp"

#include "Exceptions/NotImplementedException.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    Ref<Shader> Shader::create(InputFileStream& vertShaderStream,
                               InputFileStream& fragShaderStream) {
        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return std::make_shared<OpenGLShader>(
                    OpenGLShader(vertShaderStream, fragShaderStream));

            case RenderAPI::DirectX11:
                throw NotImplementedException();
                // return
                // std::make_shared<DX11Shader>(DX11Shader(vertShaderStream,
                // fragShaderStream);

            case RenderAPI::DirectX12:
                throw NotImplementedException();
                // return
                // std::make_shared<DX12Shader>(DX12Shader(vertShaderStream,
                // fragShaderStream);

            case RenderAPI::Vulkan:
                throw NotImplementedException();
                // return
                // std::make_shared<VulkanShader>(VulkanShader(vertShaderStream,
                // fragShaderStream);

            default:
                AE_CORE_ASSERT(false, "Unknown RendererAPI!");
                throw NotImplementedException();
        }
    }

    Ref<Shader> Shader::create(const std::string& vertShaderSrc,
                               const std::string& fragShaderSrc) {
        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return std::make_shared<OpenGLShader>(
                    OpenGLShader(vertShaderSrc, fragShaderSrc));

            case RenderAPI::DirectX11:
                throw NotImplementedException();
                // return std::make_shared<DX11Shader>(DX11Shader(vertShaderSrc,
                // fragShaderSrc);

            case RenderAPI::DirectX12:
                throw NotImplementedException();
                // return std::make_shared<DX12Shader>(DX12Shader(vertShaderSrc,
                // fragShaderSrc);

            case RenderAPI::Vulkan:
                throw NotImplementedException();
                // return
                // std::make_shared<VulkanShader>(VulkanShader(vertShaderSrc,
                // fragShaderSrc);

            default:
                AE_CORE_ASSERT(false, "Unknown RendererAPI!");
                throw NotImplementedException();
        }
    }
}  // namespace AnEngine
