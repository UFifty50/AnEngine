#include "aepch.hpp"

#include "Renderer/FrameBuffer.hpp"

#include "Exceptions/NotImplementedException.hpp"
#include "Platform/OpenGL/OpenGLFrameBuffer.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    Ref<FrameBuffer> FrameBuffer::create(const FrameBufferSpec& spec) {
        AE_PROFILE_FUNCTION()

        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return MakeRef<OpenGLFrameBuffer>(spec);

            case RenderAPI::DirectX11:
                throw NotImplementedException();
                // return MakeRef<DX11FrameBuffer>(spec);

            case RenderAPI::DirectX12:
                throw NotImplementedException();
                // return MakeRef<DX12FrameBuffer>(spec);

            case RenderAPI::Vulkan:
                throw NotImplementedException();
                // return MakeRef<VulkanFrameBuffer>(spec);

            default:
                AE_CORE_ASSERT(false, "Unknown RendererAPI!");
                throw NotImplementedException();
        }
    }
}  // namespace AnEngine
