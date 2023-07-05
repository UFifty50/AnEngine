#include "aepch.hpp"

#include "Renderer/Renderer.hpp"

#include <memory>
#include <vector>

#include "Renderer/Camera/OrthographicCamera.hpp"
#include "Renderer/RenderCommandQueue.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

    void Renderer::beginScene(OrthographicCamera& camera) {
        sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }

    void Renderer::endScene() {}

    void Renderer::submit(const std::shared_ptr<Shader>& shader,
                          const std::shared_ptr<VertexArray>& vertexArray,
                          const glm::mat4& transform,
                          const AnEngine::ShaderUniformVector& uniforms) {
        shader->bind();
        shader->uploadUniform("viewProjectionMatrix",
                              sceneData->viewProjectionMatrix);
        shader->uploadUniform("modelMatrix", transform);

        for (const auto& item : uniforms) {
            shader->uploadUniform(item.name, item.uniform);
        }
        vertexArray->bind();
        RenderCommandQueue::drawIndexed(vertexArray);
    }
};  // namespace AnEngine
