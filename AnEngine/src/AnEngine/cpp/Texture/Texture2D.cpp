#include "aepch.hpp"

#include "Texture/Texture2D.hpp"

#include <string>

#include "Core/Core.hpp"
#include "Platform/OpenGL/Texture/OpenGLTexture2D.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    Ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height) {
        AE_PROFILE_FUNCTION()

        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return MakeRef<OpenGLTexture2D>(width, height);
        };

        AE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    // Ref<Texture2D> Texture2D::create(const std::string& path) { create(fs::path(path)); }

    Ref<Texture2D> Texture2D::create(const fs::path& path) {
        AE_PROFILE_FUNCTION()

        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return MakeRef<OpenGLTexture2D>(path);
        };

        AE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
};  // namespace AnEngine
