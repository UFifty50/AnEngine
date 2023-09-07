#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <glm/glm.hpp>

#include "Texture/SpriteSheet.hpp"
#include "Texture/Texture2D.hpp"


namespace AnEngine {
    class Sprite {
    public:
        Sprite(const Ref<SpriteSheet>& sheet, glm::vec2 index);

        void render(glm::vec3 position, float rotation) const;

    private:
        glm::vec2 index;
        Ref<SpriteSheet> sheet;
    };
}  // namespace AnEngine


#endif
