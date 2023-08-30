#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

#include <glm/glm.hpp>

#include "Texture/Sprite.hpp"
#include "Texture/Texture2D.hpp"


namespace AnEngine {
    class SpriteSheet {
    public:
        SpriteSheet(const std::string& path, int spriteWidth, int spriteHeight);
        SpriteSheet(const Ref<Texture2D>& path, int spriteWidth, int spriteHeight);
        ~SpriteSheet() = default;

        void renderSprite(int index, glm::vec3 position, float rotation) const;
        Sprite getSprite(int index) const;

    private:
        int spriteWidth;
        int spriteHeight;

        Ref<Texture2D> sheet;
    };
}  // namespace AnEngine

#endif
