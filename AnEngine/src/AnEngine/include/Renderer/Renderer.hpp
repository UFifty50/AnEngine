#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>
#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    class Renderer {
    public:
        static void beginScene();
        static void endScene();
        static void submit(const std::shared_ptr<VertexArray>& vertexArray);
    };
}

#endif
