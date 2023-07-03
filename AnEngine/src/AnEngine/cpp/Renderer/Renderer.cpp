#include "aepch.hpp"

#include <memory>
#include "Renderer/Renderer.hpp"
#include "Renderer/RenderCommandQueue.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Camera/OrthographicCamera.hpp"


namespace AnEngine {
    Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

    void Renderer::beginScene(OrthographicCamera& camera) {
        sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }

    void Renderer::endScene() {
    }

    void Renderer::submit(const std::shared_ptr<VertexArray>& vertexArray) {
        vertexArray->bind();
        RenderCommandQueue::drawIndexed(vertexArray);
    }

    void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const std::initializer_list<ShaderUniformBase>& uniforms) {
        shader->bind();
        shader->uploadUniform("viewProjection", sceneData->viewProjectionMatrix);

        for (const auto& item : uniforms) {
            shader->uploadUniform(item.name, item.uniform);
        }
        vertexArray->bind();
        RenderCommandQueue::drawIndexed(vertexArray);
    }
};
