#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>
#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/ShaderUniform.hpp"
#include "Renderer/Camera/OrthographicCamera.hpp"


namespace AnEngine {
    class Renderer {
    public:
        static void beginScene(OrthographicCamera& camera);
        static void endScene();
        static void submit(const std::shared_ptr<VertexArray>& vertexArray);
        static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const std::initializer_list<ShaderUniformBase>& uniforms = {});
    
    private:
        struct SceneData {
            glm::mat4 viewProjectionMatrix;
        };

        static SceneData* sceneData;
    };
};

#endif
