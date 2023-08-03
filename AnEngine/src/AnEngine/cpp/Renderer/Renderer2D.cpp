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
    Renderer2D::Storage Renderer2D::rendererData;


    void Renderer2D::init() {
        AE_PROFILE_FUNCTION()

        rendererData.quadVA = VertexArray::create();

        rendererData.quadVB =
            VertexBuffer::create(rendererData.maxVertices * sizeof(QuadVertex));

        // clang-format off
        BufferLayout layout = {
            {ShaderDataType::Vec3, "Position"},
            {ShaderDataType::Vec4, "ColourIn"},
            {ShaderDataType::Vec2, "TexCoordIn"},
            {ShaderDataType::Float, "TexIndexIn"},
            {ShaderDataType::Float, "TilingFactorIn"},
            {ShaderDataType::Vec4, "TintIn"},
            {ShaderDataType::Float, "Rotation"},
        };
        // clang-format on

        rendererData.quadVB->setLayout(layout);

        rendererData.quadVA->addVertexBuffer(rendererData.quadVB);

        rendererData.quadVertexBufferBase = new QuadVertex[rendererData.maxVertices];

        Scope<uint32_t[]> quadIndices = MakeScope<uint32_t[]>(rendererData.maxIndices);

        uint32_t offset = 0;
        for (uint32_t i = 0; i < rendererData.maxIndices; i += 6) {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quadIB =
            IndexBuffer::create(quadIndices, rendererData.maxIndices);
        rendererData.quadVA->setIndexBuffer(quadIB);

        rendererData.blankTexture = Texture2D::create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        rendererData.blankTexture->setData(&whiteTextureData, sizeof(uint32_t));


        std::array<Sampler2D, rendererData.maxTextureSlots> samplers;
        for (uint32_t i = 0; i < rendererData.maxTextureSlots; i++) {
            samplers[i] = Sampler2D{i};
        }


        Ref<Shader> quadShader =
            rendererData.shaderLibrary.load("QuadShader", "assets/shaders/quad.glsl");
        quadShader->bind();
        quadShader->uploadUniform("textureSamplers", samplers);

        // Set texture slot 0 to blank texture
        rendererData.textureSlots[0] = rendererData.blankTexture;
    }

    void Renderer2D::beginScene(const Ref<Camera>& camera) {  // begin batch
        AE_PROFILE_FUNCTION()

        AE_CORE_ASSERT(camera->getType() == CameraType::Orthographic,
                       "Renderer2D only supports Orthographic Cameras!");

        Ref<Shader> quadShader = rendererData.shaderLibrary.get("QuadShader");

        quadShader->bind();
        quadShader->uploadUniform("viewProjectionMatrix",
                                  camera->getViewProjectionMatrix());

        rendererData.quadIndexCount = 0;
        rendererData.quadVertexBufferPtr = rendererData.quadVertexBufferBase;

        rendererData.textureSlotIndex = 1;
    }

    void Renderer2D::shutdown() {}

    void Renderer2D::endScene() {  // draw batch
        AE_PROFILE_FUNCTION()

        uint32_t dataSize = (uint8_t*)rendererData.quadVertexBufferPtr -
                            (uint8_t*)rendererData.quadVertexBufferBase;
        rendererData.quadVB->setData(rendererData.quadVertexBufferBase, dataSize);

        flush();
    }

    void Renderer2D::flush() {
        AE_PROFILE_FUNCTION()

        // Bind textures
        for (uint32_t i = 0; i < rendererData.textureSlotIndex; i++) {
            rendererData.textureSlots[i]->bind(i);
        }

        RenderCommandQueue::drawIndexed(rendererData.quadVA, rendererData.quadIndexCount);
    }

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

        constexpr float texIndex = 0.0f;
        constexpr float tilingFactor = 1.0f;
        constexpr glm::vec4 tint(1.0f);

        rendererData.quadVertexBufferPtr->position = position;
        rendererData.quadVertexBufferPtr->colour = colour;
        rendererData.quadVertexBufferPtr->texCoord = {0.0f, 0.0f};
        rendererData.quadVertexBufferPtr->texIndex = texIndex;
        rendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        rendererData.quadVertexBufferPtr->tint = tint;
        rendererData.quadVertexBufferPtr->rotation = rotation;
        rendererData.quadVertexBufferPtr++;

        rendererData.quadVertexBufferPtr->position = {position.x + size.x, position.y,
                                                      position.z};
        rendererData.quadVertexBufferPtr->colour = colour;
        rendererData.quadVertexBufferPtr->texCoord = {1.0f, 0.0f};
        rendererData.quadVertexBufferPtr->texIndex = texIndex;
        rendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        rendererData.quadVertexBufferPtr->tint = tint;
        rendererData.quadVertexBufferPtr->rotation = rotation;
        rendererData.quadVertexBufferPtr++;

        rendererData.quadVertexBufferPtr->position = {position.x + size.x,
                                                      position.y + size.y, position.z};
        rendererData.quadVertexBufferPtr->colour = colour;
        rendererData.quadVertexBufferPtr->texCoord = {1.0f, 1.0f};
        rendererData.quadVertexBufferPtr->texIndex = texIndex;
        rendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        rendererData.quadVertexBufferPtr->tint = tint;
        rendererData.quadVertexBufferPtr->rotation = rotation;
        rendererData.quadVertexBufferPtr++;

        rendererData.quadVertexBufferPtr->position = {position.x, position.y + size.y,
                                                      position.z};
        rendererData.quadVertexBufferPtr->colour = colour;
        rendererData.quadVertexBufferPtr->texCoord = {0.0f, 1.0f};
        rendererData.quadVertexBufferPtr->texIndex = texIndex;
        rendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        rendererData.quadVertexBufferPtr->tint = tint;
        rendererData.quadVertexBufferPtr->rotation = rotation;
        rendererData.quadVertexBufferPtr++;

        rendererData.quadIndexCount += 6;

        /* Ref<Shader> quadShader = rendererData->shaderLibrary.get("QuadShader");

         quadShader->uploadUniform("tint", glm::vec4(1.0f));
         quadShader->uploadUniform("tilingFactor", 1.0f);
         rendererData->blankTexture->bind();


         glm::mat4 rotationMatrix;
         if ((int)rotation % 180 != 0) {
             glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f});
         } else {
             rotationMatrix = glm::mat4(1.0f);
         }

         glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * rotationMatrix
         * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

         quadShader->uploadUniform("modelMatrix", transform);

         rendererData->quadVA->bind();
         RenderCommandQueue::drawIndexed(rendererData->quadVA);*/
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

        constexpr glm::vec4 colour(1.0f);

        float textureIndex = 0.0f;

        for (uint32_t i = 1; i < rendererData.textureSlotIndex; i++) {
            if (rendererData.textureSlots[i]->getSampler() == texture->getSampler()) {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f) {
            textureIndex = (float)rendererData.textureSlotIndex;
            rendererData.textureSlots[rendererData.textureSlotIndex] = texture;
            rendererData.textureSlotIndex++;
        }

        float tilingFactor = attributes.getOr("tilingFactor", 1.0f);
        glm::vec4 tint = attributes.getOr("tint", glm::vec4(1.0f));


        rendererData.quadVertexBufferPtr->position = position;
        rendererData.quadVertexBufferPtr->colour = colour;
        rendererData.quadVertexBufferPtr->texCoord = {0.0f, 0.0f};
        rendererData.quadVertexBufferPtr->texIndex = textureIndex;
        rendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        rendererData.quadVertexBufferPtr->tint = tint;
        rendererData.quadVertexBufferPtr->rotation = rotation;
        rendererData.quadVertexBufferPtr++;

        rendererData.quadVertexBufferPtr->position = {position.x + size.x, position.y,
                                                      position.z};
        rendererData.quadVertexBufferPtr->colour = colour;
        rendererData.quadVertexBufferPtr->texCoord = {1.0f, 0.0f};
        rendererData.quadVertexBufferPtr->texIndex = textureIndex;
        rendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        rendererData.quadVertexBufferPtr->tint = tint;
        rendererData.quadVertexBufferPtr->rotation = rotation;
        rendererData.quadVertexBufferPtr++;

        rendererData.quadVertexBufferPtr->position = {position.x + size.x,
                                                      position.y + size.y, position.z};
        rendererData.quadVertexBufferPtr->colour = colour;
        rendererData.quadVertexBufferPtr->texCoord = {1.0f, 1.0f};
        rendererData.quadVertexBufferPtr->texIndex = textureIndex;
        rendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        rendererData.quadVertexBufferPtr->tint = tint;
        rendererData.quadVertexBufferPtr->rotation = rotation;
        rendererData.quadVertexBufferPtr++;

        rendererData.quadVertexBufferPtr->position = {position.x, position.y + size.y,
                                                      position.z};
        rendererData.quadVertexBufferPtr->colour = colour;
        rendererData.quadVertexBufferPtr->texCoord = {0.0f, 1.0f};
        rendererData.quadVertexBufferPtr->texIndex = textureIndex;
        rendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        rendererData.quadVertexBufferPtr->tint = tint;
        rendererData.quadVertexBufferPtr->rotation = rotation;
        rendererData.quadVertexBufferPtr++;

        rendererData.quadIndexCount += 6;

        /*Ref<Shader> quadShader = rendererData.shaderLibrary.get("QuadShader");

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

        rendererData.quadVA->bind();
        RenderCommandQueue::drawIndexed(rendererData.quadVA);*/
    }
}  // namespace AnEngine
