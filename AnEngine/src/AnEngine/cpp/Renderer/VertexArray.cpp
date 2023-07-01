#include "aepch.hpp"

#include <Exceptions/NotImplementedException.hpp>
#include "Renderer/VertexArray.hpp"
#include "Renderer/Renderer.hpp"
#include "Platform/OpenGL/openGLVertexArray.hpp"


namespace AnEngine {
    VertexArray* VertexArray::create() {
        switch (Renderer::getAPI()) {
        case RenderAPI::OpenGL:
            return new OpenGLVertexArray();

        case RenderAPI::DirectX11:
            throw NotImplementedException();
            //     return new DX11VertexArray();

        case RenderAPI::DirectX12:
            throw NotImplementedException();
            //      return new DX12VertexArray();

        case RenderAPI::Vulkan:
            throw NotImplementedException();
            //     return new VulkanVertexArray();

        default:
            AE_CORE_ASSERT(false, "Unknown RendererAPI!");
            throw NotImplementedException();
        }
    }
};
