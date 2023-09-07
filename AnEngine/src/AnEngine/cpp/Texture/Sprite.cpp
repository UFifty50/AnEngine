#include "aepch.hpp"

#include <glm/glm.hpp>

#include "Texture/Sprite.hpp"

#include "Renderer/Renderer2D.hpp"
#include "Texture/SpriteSheet.hpp"


namespace AnEngine {
    Sprite::Sprite(const Ref<SpriteSheet>& sheet, glm::vec2 index)
        : sheet(sheet), index(index) {}

    void Sprite::render(glm::vec3 position, float rotation) const {
        sheet->renderSprite(index, position, rotation);
    }
}  // namespace AnEngine
