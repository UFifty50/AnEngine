#include "aepch.hpp"

#include "Renderer/RenderCommandQueue.hpp"
#include "Renderer/RenderAPI.hpp"

#include "Platform/OpenGL/OpenGLRenderAPI.hpp"


namespace AnEngine {
    RenderAPI* RenderCommandQueue::api = new OpenGLRenderAPI; // RenderAPI::create();
}
