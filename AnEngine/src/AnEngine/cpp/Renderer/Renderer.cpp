#include "aepch.hpp"

#include <memory>
#include "Renderer/Renderer.hpp"
#include "Renderer/RenderCommandQueue.hpp"
#include "Renderer/VertexArray.hpp"


namespace AnEngine
{
    void Renderer::beginScene() {
    }

    void Renderer::endScene() {
    }

    void Renderer::submit(const std::shared_ptr<VertexArray>& vertexArray) {
        vertexArray->bind();
        RenderCommandQueue::drawIndexed(vertexArray);
    }
}
