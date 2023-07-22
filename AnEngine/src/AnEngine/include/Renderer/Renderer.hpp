#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>
#include <vector>

#include "Renderer/Camera/Camera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/ShaderUniform.hpp"
#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    class Renderer {
    public:
        static void init();
        static void onWindowResize(uint32_t width, uint32_t height);
        static void beginScene(Ref<Camera> camera);
        static void endScene();

        static void submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
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
