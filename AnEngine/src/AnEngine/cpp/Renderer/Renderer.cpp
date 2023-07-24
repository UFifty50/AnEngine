#include "aepch.hpp"

#include "Renderer/Renderer.hpp"

#include <memory>
#include <vector>

#include "Renderer/Camera/OrthographicCamera.hpp"
#include "Renderer/RenderCommandQueue.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    Ref<Renderer::SceneData> Renderer::sceneData = MakeRef<Renderer::SceneData>();

    void Renderer::init() {
        AE_PROFILE_FUNCTION()

        RenderCommandQueue::init();
        Renderer2D::init();
    }

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
        AE_PROFILE_FUNCTION()

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
