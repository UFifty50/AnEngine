#include "aepch.hpp"

#include "Renderer/Renderer.hpp"

#include <memory>
#include <vector>

#include "Renderer/Camera/OrthographicCamera.hpp"
#include "Renderer/RenderCommandQueue.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    Ref<Renderer::SceneData> Renderer::sceneData =
        std::make_shared<Renderer::SceneData>();

    void Renderer::init() { RenderCommandQueue::init(); }

    void Renderer::beginScene(Ref<Camera> camera) {
        sceneData->viewProjectionMatrix = camera->getViewProjectionMatrix();
    }

    void Renderer::onWindowResize(uint32_t width, uint32_t height) {
        RenderCommandQueue::setViewport(0, 0, width, height);
    }

    void Renderer::endScene() {}

    void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
                          const glm::mat4& transform,
                          const AnEngine::ShaderUniformVector& uniforms) {
        shader->bind();
        shader->uploadUniform("viewProjectionMatrix", sceneData->viewProjectionMatrix);
        shader->uploadUniform("modelMatrix", transform);

        for (const auto& item : uniforms) {
            shader->uploadUniform(item.name, item.uniform);
        }
        vertexArray->bind();
        RenderCommandQueue::drawIndexed(vertexArray);
    }
};  // namespace AnEngine
