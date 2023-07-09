#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>
#include <vector>

#include "Renderer/Camera/OrthographicCamera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/ShaderUniform.hpp"
#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    class Renderer {
    public:
        static void beginScene(OrthographicCamera& camera);
        static void endScene();

        static void submit(const Ref<Shader>& shader,
                           const Ref<VertexArray>& vertexArray,
                           const glm::mat4& transform,
                           const AnEngine::ShaderUniformVector& uniforms = {});

    private:
        struct SceneData {
            glm::mat4 viewProjectionMatrix;
        };

        static Ref<SceneData> sceneData;
    };
};  // namespace AnEngine

#endif
