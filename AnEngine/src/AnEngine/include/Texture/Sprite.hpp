#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <glm/glm.hpp>

#include "Texture/SpriteSheet.hpp"
#include "Texture/Texture2D.hpp"


namespace AnEngine {
    class Sprite {
    public:
        Sprite() = default;
        Sprite(const Ref<SpriteSheet>& sheet, glm::vec2 index);

        void render(glm::vec3 position, float rotation,
                    glm::vec2 scale = {1.0f, 1.0f}) const;

    private:
        glm::vec2 index;
        Ref<SpriteSheet> sheet;
    };
}  // namespace AnEngine


#endif
