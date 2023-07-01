#ifndef RENDERCOMMANDQUEUE_HPP
#define RENDERCOMMANDQUEUE_HPP

#include <cstdint>
#include "Renderer/RenderAPI.hpp"


namespace AnEngine {
    class RenderCommandQueue {
    public:
        inline static void clearColour(const glm::vec4& colour) {
            api->clearColour(colour);
        }
        
        inline static void clear() {
            api->clear();
        }
        
        inline static void clearDepth() {
            api->clearDepth();
        }

        inline static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
            api->drawIndexed(vertexArray);
        };

    private:
        static RenderAPI* api;
    };
}

#endif
