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

        uint32_t offset = 0;
        for (uint32_t i = 0; i < MaterialBatch::maxIndices; i += 6) {
            rendererData.quadIndices[(size_t)i + 0] = offset + 0;
            rendererData.quadIndices[(size_t)i + 1] = offset + 1;
            rendererData.quadIndices[(size_t)i + 2] = offset + 2;

            rendererData.quadIndices[(size_t)i + 3] = offset + 2;
            rendererData.quadIndices[(size_t)i + 4] = offset + 3;
            rendererData.quadIndices[(size_t)i + 5] = offset + 0;

            offset += 4;
        }

        rendererData.quadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
        rendererData.quadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
        rendererData.quadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
        rendererData.quadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};

        rendererData.cameraUniformBuffer =
            UniformBuffer::Create(sizeof(Renderer2D::Storage::CameraData), 0);

        rendererData.materialBatchesHead = new MatList();
    }


    void Renderer2D::newMaterialBatch(const UUID& materialUUID) {
        AE_PROFILE_FUNCTION()

        MaterialBatch batch;
        {
            AE_PROFILE_SCOPE("AAAAAAAAAAAAAA")
            // batch.quadVBOVec.reserve(MaterialBatch::maxVertices);
            //     batch.quadVBOVec.resize(MaterialBatch::maxVertices);
        } {
            AE_PROFILE_SCOPE("Create material batch")

            batch.quadVAO = VertexArray::create();
            batch.quadVBO =
                VertexBuffer::create(MaterialBatch::maxVertices * sizeof(QuadVertex));
        }

        {
            AE_PROFILE_SCOPE("Indicie buffer stuff")
            BufferLayout layout = {
                {ShaderDataType::Vec3, "Position"},        {ShaderDataType::Vec4, "ColourIn"},
                {ShaderDataType::Vec2, "TexCoordIn"},      {ShaderDataType::Int, "TexIndexIn"},
                {ShaderDataType::Float, "TilingFactorIn"}, {ShaderDataType::Vec4, "TintIn"},
                {ShaderDataType::Int, "EntityID"}};

            batch.quadVBO->setLayout(layout);
            batch.quadVAO->addVertexBuffer(batch.quadVBO);

            Ref<IndexBuffer> quadIBO = IndexBuffer::create(rendererData.quadIndices.data(),
                                                           MaterialBatch::maxIndices);
            batch.quadVAO->setIndexBuffer(quadIBO);
        }


        {
            AE_PROFILE_SCOPE("Other stuff") {
                AE_PROFILE_SCOPE("Quad vertex buffer")
                batch.indexCount = 0;

                // Set texture slot 0 to blank texture
                batch.textureSlots[0] = rendererData.blankTexture;
                batch.textureSlotIndex = 1;
            }

            {
                AE_PROFILE_SCOPE("Add material batch")
                if (rendererData.materialBatchesHead->materialUUID.isNull()) {
                    rendererData.materialBatchesHead->materialUUID = materialUUID;
                    rendererData.materialBatchesHead->batches.push_back(batch);
                } else {
                    (*rendererData.materialBatchesHead)[materialUUID].push_back(batch);
                }
            }

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
        AE_PROFILE_FUNCTION()

        AE_CORE_ASSERT(!rendererData.activeScene,
                       "Renderer2D::shutdown() called with active scene!");

        delete rendererData.materialBatchesHead;
        rendererData.activeMaterialUUID = UUID(nullptr);

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

        {
            AE_PROFILE_SCOPE("Clear material batches")
            rendererData.materialBatchesHead->clearUnused();
            rendererData.materialBatchesHead->resetBatches();
        }

        rendererStats.usedMaterials = (uint32_t)rendererData.materialBatchesHead->size();
        rendererData.activeMaterialUUID = UUID(nullptr);

        rendererData.activeScene = false;
    }

    void Renderer2D::newBatch() {
        AE_PROFILE_FUNCTION()
        endActiveBatch();
        flushActiveBatch();

        newMaterialBatch(rendererData.activeMaterialUUID);
    }

    void Renderer2D::endActiveBatch() {
        AE_PROFILE_FUNCTION()

        MaterialBatch& batch =
            rendererData.materialBatchesHead->getBatch(rendererData.activeMaterialUUID);

        uint32_t dataSize = batch.quadVBOVec.size() * sizeof(QuadVertex);
        batch.quadVBO->setData(batch.quadVBOVec.data(), dataSize);
    }

    void Renderer2D::flushActiveBatch() {
        AE_PROFILE_FUNCTION()

        MaterialBatch& batch =
            rendererData.materialBatchesHead->getBatch(rendererData.activeMaterialUUID);

        // TODO: Bind the materials shader instead of ours
        g_ShaderLibrary.get("QuadShader")->bind();

        // Bind textures
        for (uint32_t i = 0; i < batch.textureSlotIndex; i++) {
            batch.textureSlots[i]->bind(i);
        }

        RenderCommandQueue::drawIndexed(batch.quadVAO, batch.indexCount);
        rendererStats.draws++;
    }

    void Renderer2D::endAllBatches() {
        AE_PROFILE_FUNCTION()

        for (const auto& [materialUUID, batches] : *rendererData.materialBatchesHead) {
            rendererData.activeMaterialUUID = materialUUID;
            endActiveBatch();
        }
    }

    void Renderer2D::flushAllBatches() {
        AE_PROFILE_FUNCTION()

        for (const auto& [materialUUID, batch] : *rendererData.materialBatchesHead) {
            rendererData.activeMaterialUUID = materialUUID;
            flushActiveBatch();
        }
    }


    // Primitives
    void Renderer2D::drawSprite(const glm::mat4& transform,
                                const SpriteRendererComponent& sprite, int32_t entityID) {
        AE_PROFILE_FUNCTION()

        if (rendererData.activeMaterialUUID != sprite.materialUUID &&
            !rendererData.materialBatchesHead->contains(sprite.materialUUID))
            newMaterialBatch(sprite.materialUUID);

        rendererData.activeMaterialUUID = sprite.materialUUID;
        const Material& mat = g_ActiveProject.loadMaterial(sprite.materialUUID);

        if (const auto& tex = mat.getTexture()) {
            drawQuad(transform, *tex, entityID);
        } else {
            drawQuad(transform, mat.colour, entityID);
        }
    }

    void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& colour,
                              int32_t entityID) {
        AE_PROFILE_FUNCTION()

        if (!rendererData.activeScene) {
            AE_CORE_ERROR("Renderer2D::drawQuad() called without active scene!");
            return;
        }

        MaterialBatch& batch =
            rendererData.materialBatchesHead->getBatch(rendererData.activeMaterialUUID);

        if (batch.indexCount >= MaterialBatch::maxIndices) {
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
            QuadVertex v;
            v.position = transform * rendererData.quadVertexPositions[i];
            v.colour = colour;
            v.texCoord = texCoords[i];
            v.albedoIndex = texIndex;
            v.tilingFactor = tilingFactor;
            v.tint = tint;
            v.entityID = entityID;

            batch.quadVBOVec.push_back(v);
        }

        batch.indexCount += 6;
        rendererStats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture,
                              int32_t entityID, const ShaderUniformVector& attributes) {
        AE_PROFILE_FUNCTION()

        if (!rendererData.activeScene) {
            AE_CORE_ERROR("Renderer2D::drawQuad() called without active scene!");
            return;
        }

        MaterialBatch& batch =
            rendererData.materialBatchesHead->getBatch(rendererData.activeMaterialUUID);

        if (batch.indexCount >= MaterialBatch::maxIndices ||
            batch.textureSlotIndex >= MaterialBatch::maxTextureSlots) {
            newBatch();
        }

        constexpr glm::vec4 colour(1.0f);
        uint32_t textureIndex = 0;

        for (uint32_t i = 1; i < batch.textureSlotIndex; i++) {
            if (batch.textureSlots[i]->getSampler() == texture->getSampler()) {
                textureIndex = i;
                break;
            }
        }

        if (textureIndex == 0.0f) {
            AE_CORE_ASSERT(batch.textureSlotIndex < MaterialBatch::maxTextureSlots,
                           "Renderer2D::drawQuad() exceeded max texture slots per draw call!");

            textureIndex = batch.textureSlotIndex;
            batch.textureSlots[batch.textureSlotIndex] = texture;
            batch.textureSlotIndex++;
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
            QuadVertex v;
            v.position = transform * rendererData.quadVertexPositions[i];
            v.colour = colour;
            v.texCoord = texCoords[i];
            v.albedoIndex = textureIndex;
            v.tilingFactor = tilingFactor;
            v.tint = tint;
            v.entityID = entityID;

            batch.quadVBOVec.push_back(v);
        }

        batch.indexCount += 6;
        rendererStats.quadCount++;
    }
}  // namespace AnEngine
