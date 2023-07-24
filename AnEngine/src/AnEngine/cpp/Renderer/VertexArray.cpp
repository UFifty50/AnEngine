#include "aepch.hpp"

#include <Exceptions/NotImplementedException.hpp>

#include "Renderer/VertexArray.hpp"

#include "Platform/OpenGL/openGLVertexArray.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    Ref<VertexArray> VertexArray::create() {
        AE_PROFILE_FUNCTION()

        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return MakeRef<OpenGLVertexArray>();

            case RenderAPI::DirectX11:
                throw NotImplementedException();
                //     return
                //     MakeRef<DX11VertexArray>(DX11VertexArray());

            case RenderAPI::DirectX12:
                throw NotImplementedException();
                //      return
                //      MakeRef<DX12VertexArray>(DX12VertexArray());

            case RenderAPI::Vulkan:
                throw NotImplementedException();
                //     return
                //     MakeRef<VulkanVertexArray>(VulkanVertexArray());

            default:
                AE_CORE_ASSERT(false, "Unknown RendererAPI!");
                throw NotImplementedException();
        }
    }
};  // namespace AnEngine
