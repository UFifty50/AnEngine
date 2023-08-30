#include "aepch.hpp"

#include <glm/glm.hpp>

#include "Texture/SpriteSheet.hpp"

#include "Renderer/Renderer2D.hpp"
#include "Texture/Sprite.hpp"


namespace AnEngine {
    SpriteSheet::SpriteSheet(const std::string& path, int spriteWidth, int spriteHeight)
        : spriteWidth(spriteWidth), spriteHeight(spriteHeight) {
        sheet = Texture2D::create(path);
    }


    SpriteSheet::SpriteSheet(const Ref<Texture2D>& texture, int spriteWidth,
                             int spriteHeight)
        : spriteWidth(spriteWidth), spriteHeight(spriteHeight) {
        sheet = texture;
    }

    void SpriteSheet::renderSprite(int index, glm::vec3 position, float rotation) const {}
    Sprite SpriteSheet::getSprite(int index) const { return Sprite(); }
}  // namespace AnEngine
