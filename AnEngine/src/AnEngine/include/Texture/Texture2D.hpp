#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <string>

#include "Core.hpp"
#include "File/InputFileStream.hpp"
#include "Texture/Texture.hpp"


namespace AnEngine {
    class Texture2D : public Texture {
    public:
        static Ref<Texture2D> create(const InputFileStream& path);
    };
}  // namespace AnEngine

#endif
