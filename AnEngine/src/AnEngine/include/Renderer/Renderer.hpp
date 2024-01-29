#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>
#include <vector>

#include "Renderer/RenderCommandQueue.hpp"


namespace AnEngine {
    class Renderer {
    public:
        static void init() { RenderCommandQueue::init(); }

        static void onWindowResize(uint32_t width, uint32_t height) {
            RenderCommandQueue::setViewport(0, 0, width, height);
        }
    };
};  // namespace AnEngine

#endif
