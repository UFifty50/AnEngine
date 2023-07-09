#include "aepch.hpp"

#include "Renderer/RenderCommandQueue.hpp"

#include "Platform/OpenGL/OpenGLRenderAPI.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    Scope<RenderAPI> RenderCommandQueue::api =
        std::make_unique<OpenGLRenderAPI>();  // RenderAPI::create();
}
