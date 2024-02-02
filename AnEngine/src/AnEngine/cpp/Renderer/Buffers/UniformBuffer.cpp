#include "aepch.hpp"

#include "Renderer/Buffers/UniformBuffer.hpp"

#include "Exceptions/NotImplementedException.hpp"
#include "Platform/OpenGL/Buffers/OpenGLUniformBuffer.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
        AE_PROFILE_FUNCTION()

        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return std::make_shared<OpenGLUniformBuffer>(size, binding);

            case RenderAPI::DirectX11:
                throw NotImplementedException();
                // return MakeRef<DX11VertexBuffer>(vertices, size);

            case RenderAPI::DirectX12:
                throw NotImplementedException();
                // return MakeRef<DX12VertexBuffer>(vertices, size);

            case RenderAPI::Vulkan:
                throw NotImplementedException();
                // return MakeRef<VulkanVertexBuffer>(vertices, size);

            default:
                AE_CORE_ASSERT(false, "Unknown RendererAPI!");
                throw NotImplementedException();
        }
    }
};  // namespace AnEngine
