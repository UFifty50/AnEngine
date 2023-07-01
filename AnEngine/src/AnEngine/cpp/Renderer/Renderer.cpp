#include "aepch.hpp"

#include "Renderer/Renderer.hpp"


namespace AnEngine {
   RenderAPI Renderer::currAPI = RenderAPI::NoAPI;

   RenderAPI Renderer::getAPI() {
       return currAPI;
   }

   void Renderer::setAPI(RenderAPI api) {
       currAPI = api;
   }

    std::ostream& operator<<(std::ostream& os, const RenderAPI& api) {
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
