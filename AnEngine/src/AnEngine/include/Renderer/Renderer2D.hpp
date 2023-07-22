#ifndef RENDERER2D_HPP
#define RENDERER2D_HPP

#include <glm/glm.hpp>

#include <memory>
#include <type_traits>

#include "Renderer/Camera/Camera.hpp"
#include "Texture/Texture2D.hpp"


namespace AnEngine {
    class Renderer2D {
    public:
        struct Storage;

        static void init();
        static void shutdown();

        static void beginScene(const Ref<Camera>& camera);

        static void endScene();

        // Primitives
        static void drawQuad(const glm::vec2& position, const glm::vec2& size,
                             float rotation, const glm::vec4& colour);
        static void drawQuad(const glm::vec3& position, const glm::vec2& size,
                             float rotation, const glm::vec4& colour);

        static void drawQuad(const glm::vec2& position, const glm::vec2& size,
                             float rotation, const Ref<Texture2D>& texture);
        static void drawQuad(const glm::vec3& position, const glm::vec2& size,
                             float rotation, const Ref<Texture2D>& texture);
    };
}  // namespace AnEngine

#endif
