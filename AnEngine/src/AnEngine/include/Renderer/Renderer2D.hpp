#ifndef RENDERER2D_HPP
#define RENDERER2D_HPP

#include <glm/glm.hpp>

#include <map>
#include <memory>
#include <type_traits>

#include "Project/Resources/Scene/Components.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"
#include "Texture/Texture2D.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class Renderer2D {
    public:
        struct QuadVertex {
            glm::vec3 position;
            glm::vec4 colour;
            glm::vec2 texCoord;
            uint32_t albedoIndex;
            float tilingFactor;
            glm::vec4 tint;

            // Editor Specific
            uint32_t entityID;
        };

        struct MaterialBatch {
            static const uint32_t maxQuads = 100'000;
            static const uint32_t maxVertices = maxQuads * 4;
            static const uint32_t maxIndices = maxQuads * 6;
            static const uint32_t maxTextureSlots = 32;  // TODO: Render Capabilities

            std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
            uint8_t textureSlotIndex = 1;  // 0 = blank texture

            Ref<VertexArray> quadVAO;
            Ref<VertexBuffer> quadVBO;
            uint32_t indexCount = 0;

            std::vector<QuadVertex> quadVBOVec;
        };

        struct MatList {
            MatList* next;
            Material material;
            std::vector<MaterialBatch> batches;

            MatList() : next(nullptr), material(nullptr) {}
            MatList(const Material& material) : next(nullptr), material(material) {}

            ~MatList() {
                if (next != nullptr) delete next;
            }

            std::vector<MaterialBatch>& operator[](const Material& material) {
                if (this->material == material)
                    return this->batches;
                else if (next != nullptr)
                    return (*next)[material];
                else {
                    next = new MatList(material);
                    return next->batches;
                }
            }

            MaterialBatch& getBatch(const Material& material) {
                if (this->material == material)
                    return this->batches.back();
                else if (next != nullptr)
                    return next->getBatch(material);
                else
                    AE_CORE_ASSERT(false, "MaterialBatch not found.");
            }

            void clearUnused() {
                if (next != nullptr) {
                    if (next->batches.empty()) {
                        MatList* temp = next;
                        next = next->next;
                        temp->next = nullptr;
                        delete temp;
                    } else {
                        next->clearUnused();
                    }
                }
            }

            bool contains(const Material& material) {
                if (this->material == material)
                    return true;
                else if (next != nullptr)
                    return next->contains(material);
                else
                    return false;
            }

            uint32_t size() {
                if (next != nullptr)
                    return next->size() + batches.size();
                else
                    return batches.size();
            }

            void resetBatches() {
                for (auto& batch : batches) {
                    batch.quadVBOVec.clear();
                    batch.indexCount = 0;
                }

                if (next != nullptr) {
                    next->resetBatches();
                }
            }

            // iterator that returs [material, batches] pairs
            class MatListIterator {
                typedef std::pair<Material, std::vector<MaterialBatch>> value_type;

            public:
                MatListIterator(MatList* ptr = nullptr) : ptr(ptr) {}

                operator bool() const { return ptr != nullptr; }
                bool operator==(const MatListIterator& other) const {
                    return ptr == other.ptr;
                }
                value_type operator*() const { return {ptr->material, ptr->batches}; }

                MatListIterator operator++() {
                    ptr = ptr->next;
                    return *this;
                }

                MatListIterator operator++(int) {
                    MatListIterator tmp(*this);
                    ++ptr;
                    return tmp;
                }

            private:
                MatList* ptr;
            };

            MatListIterator begin() {
                return !material.isNull() ? MatListIterator(this) : MatListIterator(nullptr);
            }
            MatListIterator end() { return MatListIterator(nullptr); }
        };

        struct Storage {
            MatList* materialBatchesHead;
            Material activeMaterial = Material(nullptr);

            glm::vec4 quadVertexPositions[4]{};
            std::array<uint32_t, MaterialBatch::maxIndices> quadIndices;

            Ref<Texture2D> blankTexture;

            bool activeScene = false;

            struct CameraData {
                glm::mat4 viewProjection;
            };
            CameraData cameraBuffer;
            Ref<UniformBuffer> cameraUniformBuffer;
        };

        struct Statistics {
            uint32_t draws = 0;
            uint32_t usedMaterials = 0;
            uint32_t quadCount = 0;
            float lastFrameTime = 0.0f;

            void reset() {
                draws = quadCount = usedMaterials = 0;
                lastFrameTime = 0.0f;
            }
            uint32_t getTotalVertexCount() { return quadCount * 4; }
            uint32_t getTotalIndexCount() { return quadCount * 6; }
        };

        static Storage rendererData;
        static Statistics rendererStats;


        static void init();
        static void shutdown();

        static void newMaterialBatch(const Material& material);

        static void beginScene(const EditorCamera2D& camera);
        static void beginScene(const Scope<Camera>& camera, const glm::mat4& transform);
        static void endScene();

        // Primitives
        static void drawSprite(const glm::mat4& transform,
                               const SpriteRendererComponent& sprite, int32_t entityID);

        static void drawQuad(const glm::mat4& transform, const glm::vec4& colour,
                             int32_t entityID = -1);
        static void drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture,
                             int32_t entityID = -1,
                             const ShaderUniformVector& attributes = {});


        // Stats
        static Statistics& getStats() { return rendererStats; }
        static void resetStats() { rendererStats.reset(); }

    private:
        static void newBatch();
        static void endActiveBatch();
        static void flushActiveBatch();

        static void endAllBatches();
        static void flushAllBatches();
    };
}  // namespace AnEngine

#endif
