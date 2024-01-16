#include "aepch.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Renderer2D.hpp"

#include "Renderer/Buffers/BufferLayout.hpp"
#include "Renderer/Buffers/IndexBuffer.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
#include "Renderer/RenderCommandQueue.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/ShaderUniform.hpp"
#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    Renderer2D::Storage Renderer2D::rendererData;
    Renderer2D::Statistics Renderer2D::rendererStats;


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


        std::array<Sampler2D, rendererData.maxTextureSlots> samplers{};
        for (uint32_t i = 0; i < rendererData.maxTextureSlots; i++) {
            samplers[i] = Sampler2D{i};
        }


        Ref<Shader> quadShader = rendererData.shaderLibrary.load(
            "QuadShader", "builtins/assets/shaders/quad.glsl");
        quadShader->bind();
        quadShader->uploadUniform("textureSamplers", samplers);

        // Set texture slot 0 to blank texture
        rendererData.textureSlots[0] = rendererData.blankTexture;

        rendererData.quadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
        rendererData.quadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
        rendererData.quadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
        rendererData.quadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
    }

    void Renderer2D::beginScene(const EditorCamera& editorCamera) {
        AE_PROFILE_FUNCTION()

        if (rendererData.activeScene) {
            AE_CORE_ERROR(
                "Renderer2D::beginScene() called when already rendering a scene!");
            return;
        }

        glm::mat4 viewProjection = editorCamera.getViewProjectionMatrix();

        Ref<Shader> quadShader = rendererData.shaderLibrary.get("QuadShader");

        quadShader->bind();
        quadShader->uploadUniform("viewProjectionMatrix", viewProjection);

        rendererData.quadIndexCount = 0;
        rendererData.quadVertexBufferPtr = rendererData.quadVertexBufferBase;

        rendererData.textureSlotIndex = 1;

        rendererData.activeScene = true;
    }

    void Renderer2D::beginScene(const Camera& camera, const glm::mat4& transform) {
        AE_PROFILE_FUNCTION()

        if (rendererData.activeScene) {
            AE_CORE_ERROR(
                "Renderer2D::beginScene() called when already rendering a scene!");
            return;
        }

        //  AE_CORE_ASSERT(camera.getType() == ProjectionType::Orthographic,
        //                  "Renderer2D only supports Orthographic Cameras!");

        glm::mat4 viewProjection = camera.getProjectionMatrix() * glm::inverse(transform);

        Ref<Shader> quadShader = rendererData.shaderLibrary.get("QuadShader");

        quadShader->bind();
        quadShader->uploadUniform("viewProjectionMatrix", viewProjection);

        rendererData.quadIndexCount = 0;
        rendererData.quadVertexBufferPtr = rendererData.quadVertexBufferBase;

        rendererData.textureSlotIndex = 1;

        rendererData.activeScene = true;
    }

    void Renderer2D::shutdown() { delete[] rendererData.quadVertexBufferBase; }

    void Renderer2D::endScene() {  // draw batch
        AE_PROFILE_FUNCTION()

        if (!rendererData.activeScene) {
            AE_CORE_ERROR("Renderer2D::endScene() called without active scene!");
            return;
        }

        endBatch();
        flush();

        rendererData.activeScene = false;
    }

    void Renderer2D::endBatch() {
        uint32_t dataSize = (uint32_t)((uint8_t*)rendererData.quadVertexBufferPtr -
                                       (uint8_t*)rendererData.quadVertexBufferBase);
        rendererData.quadVB->setData(rendererData.quadVertexBufferBase, dataSize);
    }

    void Renderer2D::newBatch() {
        endBatch();
        flush();

        rendererData.quadIndexCount = 0;
        rendererData.quadVertexBufferPtr = rendererData.quadVertexBufferBase;

        rendererData.textureSlotIndex = 1;
    }

    void Renderer2D::flush() {
        AE_PROFILE_FUNCTION()

        // Bind textures
        for (uint32_t i = 0; i < rendererData.textureSlotIndex; i++) {
            rendererData.textureSlots[i]->bind(i);
        }

        RenderCommandQueue::drawIndexed(rendererData.quadVA, rendererData.quadIndexCount);
        rendererStats.draws++;
    }

    // Primitives
    void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& colour) {
        AE_PROFILE_FUNCTION()

        if (!rendererData.activeScene) {
            AE_CORE_ERROR("Renderer2D::drawQuad() called without active scene!");
            return;
        }

        if (rendererData.quadIndexCount >= rendererData.maxIndices) {
            newBatch();
        }

        constexpr float texIndex = 0.0f;
        constexpr float tilingFactor = 1.0f;
        constexpr glm::vec4 tint(1.0f);

        glm::vec2 texCoords[4] = {
            glm::vec2{0.0f, 0.0f},
            glm::vec2{1.0f, 0.0f},
            glm::vec2{1.0f, 1.0f},
            glm::vec2{0.0f, 1.0f},
        };

        for (uint32_t i = 0; i < 4; i++) {
            rendererData.quadVertexBufferPtr->position =
                transform * rendererData.quadVertexPositions[i];
            rendererData.quadVertexBufferPtr->colour = colour;
            rendererData.quadVertexBufferPtr->texCoord = texCoords[i];
            rendererData.quadVertexBufferPtr->texIndex = texIndex;
            rendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
            rendererData.quadVertexBufferPtr->tint = tint;
            rendererData.quadVertexBufferPtr++;
        }

        rendererData.quadIndexCount += 6;
        rendererStats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture,
                              const ShaderUniformVector& attributes) {
        AE_PROFILE_FUNCTION()

        if (!rendererData.activeScene) {
            AE_CORE_ERROR("Renderer2D::drawQuad() called without active scene!");
            return;
        }

        if (rendererData.quadIndexCount >= rendererData.maxIndices ||
            rendererData.textureSlotIndex >= rendererData.maxTextureSlots) {
            newBatch();
        }

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

        glm::mat4x2 coords = {
            glm::vec2{0.0f, 0.0f},
            glm::vec2{1.0f, 0.0f},
            glm::vec2{1.0f, 1.0f},
            glm::vec2{0.0f, 1.0f},
        };
        glm::mat4x2 texCoords = attributes.getOr("texCoords", coords);

        for (uint32_t i = 0; i < 4; i++) {
            rendererData.quadVertexBufferPtr->position =
                transform * rendererData.quadVertexPositions[i];
            rendererData.quadVertexBufferPtr->colour = colour;
            rendererData.quadVertexBufferPtr->texCoord = texCoords[i];
            rendererData.quadVertexBufferPtr->texIndex = textureIndex;
            rendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
            rendererData.quadVertexBufferPtr->tint = tint;
            rendererData.quadVertexBufferPtr++;
        }

        rendererData.quadIndexCount += 6;
        rendererStats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size,
                              float rotation, const glm::vec4& colour,
                              const AnEngine::ShaderUniformVector& attributes) {
        drawQuad({position.x, position.y, 0.0f}, size, rotation, colour, attributes);
    }

    void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size,
                              float rotation, const glm::vec4& colour,
                              const AnEngine::ShaderUniformVector& attributes) {
        AE_PROFILE_FUNCTION()

        glm::mat4 rotationMatrix(1.0f);
        if ((int)rotation % 180 != 0) {
            rotationMatrix =
                glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f});
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * rotationMatrix *
                              glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        drawQuad(transform, colour);
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

        glm::mat4 rotationMatrix(1.0f);
        if ((int)rotation % 180 != 0) {
            rotationMatrix =
                glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f});
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * rotationMatrix *
                              glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        drawQuad(transform, texture, attributes);
    }
}  // namespace AnEngine
