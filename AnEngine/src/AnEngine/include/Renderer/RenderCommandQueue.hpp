#ifndef RENDERCOMMANDQUEUE_HPP
#define RENDERCOMMANDQUEUE_HPP

#include <cstdint>

#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    class RenderCommandQueue {
    public:
        static void init() { api->init(); }

        static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
            api->setViewport(0, 0, width, height);
        }

        inline static void clearColour(const glm::vec4& colour) {
            api->clearColour(colour);
        }

        inline static void clear() { api->clear(); }

        inline static void clearDepth() { api->clearDepth(); }

        inline static void drawIndexed(const Ref<VertexArray>& vertexArray) {
            api->drawIndexed(vertexArray);
        };

    private:
        static Scope<RenderAPI> api;
    };
}  // namespace AnEngine

#endif
