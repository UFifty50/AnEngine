#include "aepch.hpp"

#include "Texture/Texture2D.hpp"

#include <string>

#include "Core.hpp"
#include "File/InputFileStream.hpp"
#include "Platform/OpenGL/Texture/OpenGLTexture2D.hpp"
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    Ref<Texture2D> Texture2D::create(const InputFileStream& file) {
        std::string data = file.readAll();

        switch (RenderAPI::getAPI()) {
            case RenderAPI::OpenGL:
                return std::make_shared<OpenGLTexture2D>(OpenGLTexture2D(data));
        };

        AE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
};  // namespace AnEngine
