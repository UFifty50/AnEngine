#include "aepch.hpp"

#include "Renderer/Buffers/VertexBuffer.hpp"

#include "Exceptions/NotImplementedException.hpp"
#include "Platform/OpenGL/Buffers/OpenGLVertexBuffer.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    Ref<VertexBuffer> VertexBuffer::create(float* vertices, size_t size) {
        AE_PROFILE_FUNCTION()

        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return MakeRef<OpenGLVertexBuffer>(vertices, size);

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

    Ref<VertexBuffer> VertexBuffer::create(std::vector<float> vertices) {
        float* verts = new float[vertices.size() + 1];
        for (size_t i = 0; i < vertices.size(); i++) {
            verts[i] = vertices[i];
        }
        return create(verts, vertices.size() + 1);
    }

    template <size_t _Size>
    Ref<VertexBuffer> VertexBuffer::create(std::array<float, _Size> vertices) {
        float* verts = new float[_Size + 1];
        for (size_t i = 0; i < vertices.size(); i++) {
            verts[i] = vertices[i];
        }
        return create(verts, _Size);
    }
}  // namespace AnEngine
