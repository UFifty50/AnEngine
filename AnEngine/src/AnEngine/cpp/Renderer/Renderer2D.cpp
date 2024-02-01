#include "aepch.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Renderer2D.hpp"

#include "Globals.hpp"
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
    ShaderLibrary g_ShaderLibrary;
    Renderer2D::Storage Renderer2D::rendererData;
    Renderer2D::Statistics Renderer2D::rendererStats;


    void Renderer2D::init() {
        AE_PROFILE_FUNCTION()

        rendererData.blankTexture = Texture2D::create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        rendererData.blankTexture->setData(&whiteTextureData, sizeof(uint32_t));

        rendererData.cameraUniformBuffer =
            UniformBuffer::Create(sizeof(Renderer2D::Storage::CameraData), 0);
    }


    void Renderer2D::newMaterialBatch(const Material& material) {
        AE_PROFILE_FUNCTION()

        rendererData.materialBatches[material].push_back(MakeScope<MaterialBatch>());
        const Scope<MaterialBatch>& batch = rendererData.materialBatches[material].back();

        batch->quadVAO = VertexArray::create();
        batch->quadVBO = VertexBuffer::create(MaterialBatch::maxVertices * sizeof(QuadVertex));

        {
            AE_PROFILE_SCOPE("Indicie buffer stuff")
            BufferLayout layout = {
                {ShaderDataType::Vec3, "Position"},        {ShaderDataType::Vec4, "ColourIn"},
                {ShaderDataType::Vec2, "TexCoordIn"},      {ShaderDataType::Int, "TexIndexIn"},
                {ShaderDataType::Float, "TilingFactorIn"}, {ShaderDataType::Vec4, "TintIn"},
                {ShaderDataType::Int, "EntityID"}};

            batch->quadVBO->setLayout(layout);
            batch->quadVAO->addVertexBuffer(batch->quadVBO);


            Scope<uint32_t[]> quadIndices = MakeScope<uint32_t[]>(MaterialBatch::maxIndices);
            uint32_t offset = 0;

            // TODO: THIS IS SLOW
            for (uint32_t i = 0; i < MaterialBatch::maxIndices; i += 6) {
                quadIndices[(size_t)i + 0] = offset + 0;
                quadIndices[(size_t)i + 1] = offset + 1;
                quadIndices[(size_t)i + 2] = offset + 2;

                quadIndices[(size_t)i + 3] = offset + 2;
                quadIndices[(size_t)i + 4] = offset + 3;
                quadIndices[(size_t)i + 5] = offset + 0;

                offset += 4;
            }

            Ref<IndexBuffer> quadIBO =
                IndexBuffer::create(quadIndices, MaterialBatch::maxIndices);
            batch->quadVAO->setIndexBuffer(quadIBO);
        }


        {
            AE_PROFILE_SCOPE("Other stuff") {
                AE_PROFILE_SCOPE("Quad vertex buffer")
                // TODO:  THIS IS SLOW
                batch->quadVBOBase = new QuadVertex[MaterialBatch::maxVertices];
                batch->quadVBOPtr = batch->quadVBOBase;
                batch->indexCount = 0;

                // Set texture slot 0 to blank texture
                batch->textureSlots[0] = rendererData.blankTexture;
                batch->textureSlotIndex = 1;
            }


            rendererData.quadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
            rendererData.quadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
            rendererData.quadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
            rendererData.quadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};


            rendererData.activeMaterial = material;

            g_ShaderLibrary.load("QuadShader", "builtins/assets/shaders/quad.glsl");
        }
    }


    void Renderer2D::beginScene(const EditorCamera2D& editorCamera) {
        AE_PROFILE_FUNCTION()

        if (rendererData.activeScene) {
            AE_CORE_ERROR("Renderer2D::beginScene() called when already rendering a scene!");
            return;
        }

        rendererData.cameraBuffer.viewProjection = editorCamera.getViewProjectionMatrix();
        rendererData.cameraUniformBuffer->setData(&rendererData.cameraBuffer,
                                                  sizeof(Renderer2D::Storage::CameraData));

        rendererData.activeScene = true;
    }


    void Renderer2D::beginScene(const Scope<Camera>& camera, const glm::mat4& transform) {
        AE_PROFILE_FUNCTION()

        if (rendererData.activeScene) {
            AE_CORE_ERROR("Renderer2D::beginScene() called when already rendering a scene!");
            return;
        }

        //  AE_CORE_ASSERT(camera.getType() == ProjectionType::Orthographic,
        //                  "Renderer2D only supports Orthographic Cameras!");

        rendererData.cameraBuffer.viewProjection =
            camera->getProjectionMatrix() * glm::inverse(transform);
        rendererData.cameraUniformBuffer->setData(&rendererData.cameraBuffer,
                                                  sizeof(Renderer2D::Storage::CameraData));

        rendererData.activeScene = true;
    }

    void Renderer2D::shutdown() {
        AE_CORE_ASSERT(!rendererData.activeScene,
                       "Renderer2D::shutdown() called with active scene!");

        rendererData.materialBatches.clear();
        rendererData.activeMaterial = Material("Empty");

        //   rendererData.textureSlotIndex = 1;
    }

    void Renderer2D::endScene() {  // draw batch
        AE_PROFILE_FUNCTION()

        if (!rendererData.activeScene) {
            AE_CORE_ERROR("Renderer2D::endScene() called without active scene!");
            return;
        }

        endAllBatches();
        flushAllBatches();

        rendererStats.usedMaterials = (uint32_t)rendererData.materialBatches.size();
        {
            AE_PROFILE_SCOPE("Clear material batches")
            rendererData.materialBatches.clear();
        }
        rendererData.activeMaterial = Material("Empty");

        rendererData.activeScene = false;
    }

    void Renderer2D::newBatch() {
        endActiveBatch();
        flushActiveBatch();

        newMaterialBatch(rendererData.activeMaterial);
    }

    void Renderer2D::endActiveBatch() {
        const Scope<MaterialBatch>& batch =
            rendererData.materialBatches[rendererData.activeMaterial].back();

        uint32_t dataSize =
            (uint32_t)((uint8_t*)batch->quadVBOPtr - (uint8_t*)batch->quadVBOBase);
        batch->quadVBO->setData(batch->quadVBOBase, dataSize);
    }

    void Renderer2D::flushActiveBatch() {
        AE_PROFILE_FUNCTION()

        const Scope<MaterialBatch>& batch =
            rendererData.materialBatches[rendererData.activeMaterial].back();

        // Bind textures
        for (uint32_t i = 0; i < batch->textureSlotIndex; i++) {
            batch->textureSlots[i]->bind(i);
        }

        // TODO: Bind the materials shader instead of ours
        g_ShaderLibrary.get("QuadShader")->bind();
        RenderCommandQueue::drawIndexed(batch->quadVAO, batch->indexCount);
        rendererStats.draws++;
    }

    void Renderer2D::endAllBatches() {
        AE_PROFILE_FUNCTION()

        for (auto& [material, batches] : rendererData.materialBatches) {
            rendererData.activeMaterial = material;
            endActiveBatch();
        }
    }

    void Renderer2D::flushAllBatches() {
        AE_PROFILE_FUNCTION()

        for (auto& [material, batch] : rendererData.materialBatches) {
            rendererData.activeMaterial = material;
            flushActiveBatch();
        }
    }


    // Primitives
    void Renderer2D::drawSprite(const glm::mat4& transform,
                                const SpriteRendererComponent& sprite, int32_t entityID) {
        if (rendererData.activeMaterial != sprite.SpriteMaterial &&
            !rendererData.materialBatches.contains(sprite.SpriteMaterial))
            newMaterialBatch(sprite.SpriteMaterial);

        if (auto tex = rendererData.activeMaterial.getTexture()) {
            drawQuad(transform, *tex, entityID);
        } else {
            drawQuad(transform, rendererData.activeMaterial.colour, entityID);
        }
    }

    void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& colour,
                              int32_t entityID) {
        AE_PROFILE_FUNCTION()

        if (!rendererData.activeScene) {
            AE_CORE_ERROR("Renderer2D::drawQuad() called without active scene!");
            return;
        }

        const Scope<MaterialBatch>& batch =
            rendererData.materialBatches[rendererData.activeMaterial].back();

        if (batch->indexCount >= MaterialBatch::maxIndices) {
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
            batch->quadVBOPtr->position = transform * rendererData.quadVertexPositions[i];
            batch->quadVBOPtr->colour = colour;
            batch->quadVBOPtr->texCoord = texCoords[i];
            batch->quadVBOPtr->albedoIndex = texIndex;
            batch->quadVBOPtr->tilingFactor = tilingFactor;
            batch->quadVBOPtr->tint = tint;
            batch->quadVBOPtr->entityID = entityID;
            batch->quadVBOPtr++;
        }

        batch->indexCount += 6;
        rendererStats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture,
                              int32_t entityID, const ShaderUniformVector& attributes) {
        AE_PROFILE_FUNCTION()

        if (!rendererData.activeScene) {
            AE_CORE_ERROR("Renderer2D::drawQuad() called without active scene!");
            return;
        }

        const Scope<MaterialBatch>& batch =
            rendererData.materialBatches[rendererData.activeMaterial].back();

        if (batch->indexCount >= MaterialBatch::maxIndices ||
            batch->textureSlotIndex >= MaterialBatch::maxTextureSlots) {
            newBatch();
        }

        constexpr glm::vec4 colour(1.0f);
        uint32_t textureIndex = 0;

        for (uint32_t i = 1; i < batch->textureSlotIndex; i++) {
            if (batch->textureSlots[i]->getSampler() == texture->getSampler()) {
                textureIndex = i;
                break;
            }
        }

        if (textureIndex == 0.0f) {
            AE_CORE_ASSERT(batch->textureSlotIndex < MaterialBatch::maxTextureSlots,
                           "Renderer2D::drawQuad() exceeded max texture slots per draw call!");

            textureIndex = batch->textureSlotIndex;
            batch->textureSlots[batch->textureSlotIndex] = texture;
            batch->textureSlotIndex++;
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
            batch->quadVBOPtr->position = transform * rendererData.quadVertexPositions[i];
            batch->quadVBOPtr->colour = colour;
            batch->quadVBOPtr->texCoord = texCoords[i];
            batch->quadVBOPtr->albedoIndex = textureIndex;
            batch->quadVBOPtr->tilingFactor = tilingFactor;
            batch->quadVBOPtr->tint = tint;
            batch->quadVBOPtr->entityID = entityID;
            batch->quadVBOPtr++;
        }

        batch->indexCount += 6;
        rendererStats.quadCount++;
    }
}  // namespace AnEngine
