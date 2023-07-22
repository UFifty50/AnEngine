#include "aepch.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Renderer2D.hpp"

#include "Renderer/Buffers/BufferLayout.hpp"
#include "Renderer/Buffers/IndexBuffer.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Renderer/Camera/OrthographicCamera.hpp"
#include "Renderer/RenderCommandQueue.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/ShaderUniform.hpp"
#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    static Scope<Renderer2D::Storage> rendererData;

    struct Renderer2D::Storage {
        Ref<VertexArray> quadVA;
        ShaderLibrary shaderLibrary;
    };


    void Renderer2D::init() {
        rendererData = MakeScope<Renderer2D::Storage>();

        rendererData->quadVA = VertexArray::create();

        // clang-format off
            float vertices[5 * 4] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
            };
        // clang-format on

        Ref<VertexBuffer> quadVB = VertexBuffer::create(vertices, sizeof(vertices));

        BufferLayout layout = {
            {ShaderDataType::Vec3, "position"},
            {ShaderDataType::Vec2, "TexCoord"},
        };
        quadVB->setLayout(layout);

        rendererData->quadVA->addVertexBuffer(quadVB);

        uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
        Ref<IndexBuffer> quadIB = IndexBuffer::create(indices, 6);

        rendererData->quadVA->setIndexBuffer(quadIB);


        rendererData->shaderLibrary.load("SmallSquares", "assets/shaders/basic.glsl");
        rendererData->shaderLibrary.load("TextureShader", "assets/shaders/texture.glsl");
    }

    void Renderer2D::beginScene(const Ref<Camera>& camera) {
        AE_CORE_ASSERT(camera->getType() == CameraType::Orthographic,
                       "Renderer2D only supports Orthographic Cameras!");

        Ref<Shader> quadShader = rendererData->shaderLibrary.get("SmallSquares");
        Ref<Shader> textureShader = rendererData->shaderLibrary.get("TextureShader");

        quadShader->bind();
        quadShader->uploadUniform("viewProjectionMatrix",
                                  camera->getViewProjectionMatrix());

        textureShader->bind();
        textureShader->uploadUniform("viewProjectionMatrix",
                                     camera->getViewProjectionMatrix());
        textureShader->uploadUniform("Utexture", Sampler2D{0});
    }

    void Renderer2D::shutdown() { rendererData.reset(nullptr); }

    void Renderer2D::endScene() {}

    // Primitives
    void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size,
                              float rotation, const glm::vec4& colour) {
        drawQuad({position.x, position.y, 0.0f}, size, rotation, colour);
    }

    void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size,
                              float rotation, const glm::vec4& colour) {
        Ref<Shader> quadShader = rendererData->shaderLibrary.get("SmallSquares");
        quadShader->bind();
        quadShader->uploadUniform("Ucolour", colour);

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
            glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        quadShader->uploadUniform("modelMatrix", transform);

        rendererData->quadVA->bind();
        RenderCommandQueue::drawIndexed(rendererData->quadVA);
    }

    void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size,
                              float rotation, const Ref<Texture2D>& texture) {
        drawQuad({position.x, position.y, 0.0f}, size, rotation, texture);
    }

    void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size,
                              float rotation, const Ref<Texture2D>& texture) {
        Ref<Shader> textureShader = rendererData->shaderLibrary.get("TextureShader");
        textureShader->bind();

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
            glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        textureShader->uploadUniform("modelMatrix", transform);
        texture->bind(0);

        rendererData->quadVA->bind();
        RenderCommandQueue::drawIndexed(rendererData->quadVA);
    }
}  // namespace AnEngine
