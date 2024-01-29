#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <filesystem>
#include <string>

#include "Core/Core.hpp"
#include "Texture/Texture.hpp"


namespace fs = std::filesystem;

namespace AnEngine {
    class Texture2D : public Texture {
    public:
        static Ref<Texture2D> create(uint32_t width, uint32_t height);
        //    static Ref<Texture2D> create(const std::string& path);
        static Ref<Texture2D> create(const fs::path& path);
    };
}  // namespace AnEngine

#endif
