#include "aepch.hpp"

#include "Renderer/RenderAPI.hpp"

#include "Exceptions/NotImplementedException.hpp"
#include "Platform/OpenGL/OpenGLRenderAPI.hpp"


namespace AnEngine {
    RenderAPI::API RenderAPI::currAPI = RenderAPI::NoAPI;

    /*Ref<RenderAPI> RenderAPI::create() {
        switch (currAPI) {
            case RenderAPI::OpenGL:
                return MakeRef<OpenGLRenderAPI>();

            case RenderAPI::DirectX11:
                throw NotImplementedException();
                // return MakeRef<DX11RenderAPI>();

            case RenderAPI::DirectX12:
                throw NotImplementedException();
                // return MakeRef<DX12RenderAPI>();

            case RenderAPI::Vulkan:
                throw NotImplementedException();
                // return MakeRef<VulkanRenderAPI>();

            default:
                AE_CORE_ASSERT(false, "Unknown RendererAPI!");
                throw NotImplementedException();
        }
    } */

    std::ostream& operator<<(std::ostream& os, const RenderAPI::API& api) {
        switch (api) {
            case RenderAPI::NoAPI:
                return os << "NoAPI";

            case RenderAPI::OpenGL:
                return os << "OpenGL";

            case RenderAPI::DirectX11:
                return os << "DirectX11";

            case RenderAPI::DirectX12:
                return os << "DirectX12";

            case RenderAPI::Vulkan:
                return os << "Vulkan";
        }

        AE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return os;
    }
}  // namespace AnEngine
