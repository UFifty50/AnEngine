#include "aepch.hpp"

#include "Texture/Texture2D.hpp"

#include <string>

#include "Core.hpp"
#include "Platform/OpenGL/Texture/OpenGLTexture2D.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    Ref<Texture2D> Texture2D::create(const std::string& path) {
        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return std::make_shared<OpenGLTexture2D>(path);
        };

        AE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
};  // namespace AnEngine
