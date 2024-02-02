#include "aepch.hpp"

#include "Renderer/Buffers/IndexBuffer.hpp"

#include "Exceptions/NotImplementedException.hpp"
#include "Platform/OpenGL/Buffers/OpenGLIndexBuffer.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    Ref<IndexBuffer> IndexBuffer::create(const uint32_t const* indices, uint32_t count) {
        AE_PROFILE_FUNCTION()

        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return MakeRef<OpenGLIndexBuffer>(indices, count);

            case RenderAPI::DirectX11:
                throw NotImplementedException();
                // return MakeRef<DX11IndexBuffer>(indices, size);

            case RenderAPI::DirectX12:
                throw NotImplementedException();
                // return MakeRef<DX12IndexBuffer>(indices, size);

            case RenderAPI::Vulkan:
                throw NotImplementedException();
                // return MakeRef<VulkanIndexBuffer>(indices, size);

            default:
                AE_CORE_ASSERT(false, "Unknown RendererAPI!");
                throw NotImplementedException();
        }
    }
}  // namespace AnEngine
