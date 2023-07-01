#include "aepch.hpp"

#include "Renderer/Buffers/IndexBuffer.hpp"
#include "Renderer/Renderer.hpp"
#include "Exceptions/NotImplementedException.hpp"

#include "Platform/OpenGL/Buffers/OpenGLIndexBuffer.hpp"


namespace AnEngine {
    IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count) {
        switch (Renderer::getAPI()) {
            case RenderAPI::OpenGL:
                return new OpenGLIndexBuffer(indices, count);

            case RenderAPI::DirectX11:
                throw NotImplementedException();
           //     return new DX11IndexBuffer(indices, size);

            case RenderAPI::DirectX12:
                 throw NotImplementedException();
          //      return new DX12IndexBuffer(indices, size);

            case RenderAPI::Vulkan:
                 throw NotImplementedException();
           //     return new VulkanIndexBuffer(indices, size);

            default:
                AE_CORE_ASSERT(false, "Unknown RendererAPI!");
                throw NotImplementedException();
        }
    }
    IndexBuffer* IndexBuffer::create(std::vector<uint32_t> indices) {
        uint32_t* idxs = new uint32_t[indices.size() + 1];
        for (uint32_t i = 0; i < indices.size(); i++) {
            idxs[i] = indices[i];
        }
        return create(idxs, indices.size());
    }
    template<uint32_t _Size> IndexBuffer* IndexBuffer::create(std::array<uint32_t, _Size> indices) {
        uint32_t* idxs = new uint32_t[_Size + 1];
        for (uint32_t i = 0; i < _Size; i++) {
            idxs[i] = indices[i];
        }
        return create(idxs, _Size);
    }
}
