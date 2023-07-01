#include "aepch.hpp"

#include "Exceptions/NotImplementedException.hpp"
#include "Renderer/RenderAPI.hpp"

#include "Platform/OpenGL/OpenGLRenderAPI.hpp"


namespace AnEngine {
    RenderAPI::API RenderAPI::currAPI = RenderAPI::NoAPI;

    RenderAPI* RenderAPI::create() {
        switch (currAPI) {
            case RenderAPI::OpenGL:
                return new OpenGLRenderAPI();
            
            case RenderAPI::DirectX11:
                throw NotImplementedException();
                // return new DX11RenderAPI();
            
            case RenderAPI::DirectX12:
                throw NotImplementedException();
                // return new DX12RenderAPI();
            
            case RenderAPI::Vulkan:
                throw NotImplementedException();
                // return new VulkanRenderAPI();
            
            default:
                AE_CORE_ASSERT(false, "Unknown RendererAPI!");
                throw NotImplementedException();
        }   
    }
    
    std::ostream& operator<<(std::ostream& os, const RenderAPI::API& api) {
        switch (api) {
            case RenderAPI::NoAPI:
                os << "NoAPI";
                break;

            case RenderAPI::OpenGL:
                os << "OpenGL";
                break;

            case RenderAPI::DirectX11:
                os << "DirectX11";
                break;

            case RenderAPI::DirectX12:
                os << "DirectX12";
                break;

            case RenderAPI::Vulkan:
                os << "Vulkan";
                break;

            default:
                break;
        }

        return os;
    }
}
