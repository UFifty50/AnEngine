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
        Ref<Texture2D> blankTexture;
        ShaderLibrary shaderLibrary;
    };


    void Renderer2D::init() {
        AE_PROFILE_FUNCTION()

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

        rendererData->blankTexture = Texture2D::create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        rendererData->blankTexture->setData(&whiteTextureData, sizeof(uint32_t));

        Ref<Shader> quadShader =
            rendererData->shaderLibrary.load("QuadShader", "assets/shaders/quad.glsl");
        quadShader->bind();
        quadShader->uploadUniform("textureSampler", Sampler2D{0});
    }

    void Renderer2D::beginScene(const Ref<Camera>& camera) {
        AE_PROFILE_FUNCTION()

        AE_CORE_ASSERT(camera->getType() == CameraType::Orthographic,
                       "Renderer2D only supports Orthographic Cameras!");

        Ref<Shader> quadShader = rendererData->shaderLibrary.get("QuadShader");

        quadShader->bind();
        quadShader->uploadUniform("viewProjectionMatrix",
                                  camera->getViewProjectionMatrix());
    }

    void Renderer2D::shutdown() { rendererData.reset(nullptr); }

    void Renderer2D::endScene() { AE_PROFILE_FUNCTION() }

    // Primitives
    void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size,
                              float rotation, const glm::vec4& colour,
                              const AnEngine::ShaderUniformVector& attributes) {
        drawQuad({position.x, position.y, 0.0f}, size, rotation, colour, attributes);
    }

    void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size,
                              float rotation, const glm::vec4& colour,
                              const AnEngine::ShaderUniformVector& attributes) {
        AE_PROFILE_FUNCTION()

        Ref<Shader> quadShader = rendererData->shaderLibrary.get("QuadShader");

        quadShader->uploadUniform("Ucolour", colour);
        quadShader->uploadUniform("tint", glm::vec4(1.0f));
        quadShader->uploadUniform("tilingFactor", 1.0f);
        rendererData->blankTexture->bind();


        glm::mat4 rotationMatrix;
        if ((int)rotation % 180 != 0) {
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f});
        } else {
            rotationMatrix = glm::mat4(1.0f);
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * rotationMatrix *
                              glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        quadShader->uploadUniform("modelMatrix", transform);

        rendererData->quadVA->bind();
        RenderCommandQueue::drawIndexed(rendererData->quadVA);
    }

    void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size,
                              float rotation, const Ref<Texture2D>& texture,
                              const AnEngine::ShaderUniformVector& attributes) {
        drawQuad({position.x, position.y, 0.0f}, size, rotation, texture, attributes);
    }

    void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size,
                              float rotation, const Ref<Texture2D>& texture,
                              const AnEngine::ShaderUniformVector& attributes) {
        AE_PROFILE_FUNCTION()

        Ref<Shader> quadShader = rendererData->shaderLibrary.get("QuadShader");

        quadShader->uploadUniform("Ucolour", glm::vec4(1.0f));

        quadShader->uploadUniform("tint", attributes.getOr("tint", glm::vec4(1.0f)));
        quadShader->uploadUniform("tilingFactor", attributes.getOr("tilingFactor", 1.0f));
        texture->bind();

        glm::mat4 rotationMatrix;
        if ((int)rotation % 180 != 0) {
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f});
        } else {
            rotationMatrix = glm::mat4(1.0f);
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * rotationMatrix *
                              glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        quadShader->uploadUniform("modelMatrix", transform);

        rendererData->quadVA->bind();
        RenderCommandQueue::drawIndexed(rendererData->quadVA);
    }
}  // namespace AnEngine
