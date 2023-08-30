#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <glm/glm.hpp>

#include "Texture/Texture2D.hpp"


namespace AnEngine {
    class Sprite {
    public:
        Sprite();
        Sprite(const std::string& path);
        Sprite(const Ref<Texture2D>& texture);

        void render(glm::vec3 position, float rotation) const;

    private:
        Ref<Texture2D> texture;
    };
}  // namespace AnEngine


#endif
