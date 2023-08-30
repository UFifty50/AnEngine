#include "aepch.hpp"

#include <glm/glm.hpp>

#include "Texture/Sprite.hpp"

#include "Renderer/Renderer2D.hpp"


namespace AnEngine {
    Sprite::Sprite() { texture = Texture2D::create(0, 0); }
    Sprite::Sprite(const std::string& path) { texture = Texture2D::create(path); }
    Sprite::Sprite(const Ref<Texture2D>& texture) { this->texture = texture; }

    void Sprite::render(glm::vec3 position, float rotation) const {
        Renderer2D::drawQuad(
            position, {texture->getWidth() / 100.0f, texture->getHeight() / 100.0f},
            rotation, texture);
    }
}  // namespace AnEngine
