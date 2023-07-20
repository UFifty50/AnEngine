#include "aepch.hpp"

#include "Renderer/Renderer2D.hpp"

#include <memory>
#include <vector>

#include "Renderer/Camera/OrthographicCamera.hpp"
#include "Renderer/RenderCommandQueue.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    Ref<Renderer2D::SceneData> Renderer2D::sceneData =
        std::make_shared<Renderer2D::SceneData>();

    void Renderer2D::init() { RenderCommandQueue::init(); }

    void Renderer2D::beginScene(Ref<Camera> camera) {
        sceneData->viewProjectionMatrix = camera->getViewProjectionMatrix();
    }

    void Renderer2D::onWindowResize(uint32_t width, uint32_t height) {
        RenderCommandQueue::setViewport(0, 0, width, height);
    }

    void Renderer2D::endScene() {}

    void Renderer2D::submit(const Ref<Shader>& shader,
                            const Ref<VertexArray>& vertexArray,
                            const glm::mat4& transform,
                            const AnEngine::ShaderUniformVector& uniforms) {
        shader->bind();
        shader->uploadUniform("viewProjectionMatrix", sceneData->viewProjectionMatrix);
        shader->uploadUniform("modelMatrix", transform);

        for (const auto& [name, uniform] : uniforms) {
            shader->uploadUniform(name, uniform);
        }
        vertexArray->bind();
        RenderCommandQueue::drawIndexed(vertexArray);
    }
};  // namespace AnEngine
