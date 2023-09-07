#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

#include <glm/glm.hpp>

#include "Texture/Texture2D.hpp"


namespace AnEngine {
    class Sprite;

    class SpriteSheet {
    public:
        SpriteSheet(const std::string& path, int spriteWidth, int spriteHeight);
        SpriteSheet(const Ref<Texture2D>& path, int spriteWidth, int spriteHeight);
        ~SpriteSheet() = default;

        void renderSprite(glm::vec2 index, glm::vec3 position, float rotation) const;
        Sprite getSprite(glm::vec2 index) const;
        // void renderSheetWithIndices(glm::vec3 position, float rotation) const;

        const Ref<Texture2D>& getTexture() const { return sheet; }

    private:
        Ref<Texture2D> sheet;

        const float texWidth;
        const float texHeight;
        const int spritesPerRow;

        const int spriteWidth;
        const int spriteHeight;
    };
}  // namespace AnEngine

#endif
