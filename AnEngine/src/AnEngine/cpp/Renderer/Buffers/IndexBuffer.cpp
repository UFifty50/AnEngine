#include "aepch.hpp"

#include "Renderer/Buffers/IndexBuffer.hpp"

#include "Exceptions/NotImplementedException.hpp"
#include "Platform/OpenGL/Buffers/OpenGLIndexBuffer.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t count) {
        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return std::make_shared<OpenGLIndexBuffer>(indices, count);

            case RenderAPI::DirectX11:
                throw NotImplementedException();
                // return std::make_shared<DX11IndexBuffer>(indices, size);

            case RenderAPI::DirectX12:
                throw NotImplementedException();
                // return std::make_shared<DX12IndexBuffer>(indices, size);

            case RenderAPI::Vulkan:
                throw NotImplementedException();
                // return std::make_shared<VulkanIndexBuffer>(indices, size);

            default:
                AE_CORE_ASSERT(false, "Unknown RendererAPI!");
                throw NotImplementedException();
        }
    }

    Ref<IndexBuffer> IndexBuffer::create(std::vector<uint32_t> indices) {
        uint32_t* idxs = new uint32_t[indices.size() + 1];
        for (uint32_t i = 0; i < indices.size(); i++) {
            idxs[i] = indices[i];
        }
        return create(idxs, (uint32_t)indices.size());
    }

    template <uint32_t _Size>
    Ref<IndexBuffer> IndexBuffer::create(std::array<uint32_t, _Size> indices) {
        uint32_t* idxs = new uint32_t[_Size + 1];
        for (uint32_t i = 0; i < _Size; i++) {
            idxs[i] = indices[i];
        }
        return create(idxs, _Size);
    }
}  // namespace AnEngine
