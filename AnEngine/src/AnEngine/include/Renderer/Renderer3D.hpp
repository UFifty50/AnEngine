#ifndef RENDERER3D_HPP
#define RENDERER3D_HPP

#include <glm/glm.hpp>

#include <memory>
#include <type_traits>

#include "Project/Resources/Scene/Components.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"
#include "Texture/Texture2D.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class Renderer3D : public Renderer {
    public:
        struct QuadVertex {
            glm::vec3 position;
            glm::vec4 colour;
            glm::vec2 texCoord;
            float texIndex;
            float tilingFactor;
            glm::vec4 tint;

            // Editor Specific
            uint32_t entityID;
        };

        struct Storage {
            static const uint32_t maxQuads = 100'000;
            static const uint32_t maxVertices = maxQuads * 4;
            static const uint32_t maxIndices = maxQuads * 6;
            static const uint32_t maxTextureSlots = 32;  // TODO: Render Capabilities

            Ref<VertexArray> VAO;
            Ref<VertexBuffer> VBO;
            Ref<Texture2D> blankTexture;
            ShaderLibrary shaderLibrary;  // Ref<Shader> shader;

            uint32_t quadIndexCount = 0;
            QuadVertex* VBOBase = nullptr;
            QuadVertex* VBOPtr = nullptr;

            std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
            uint8_t textureSlotIndex = 1;  // 0 = blank texture

            glm::vec4 quadVertexPositions[4];

            bool activeScene = false;

            struct CameraData {
                glm::mat4 viewProjection;
            };
            CameraData cameraBuffer;
            Ref<UniformBuffer> cameraUniformBuffer;
        };

        struct Statistics {
            uint32_t draws = 0;
            uint32_t quadCount = 0;
            uint32_t materialsUsed = 0;
            float lastFrameTime = 0.0f;

            void reset() {
                draws = quadCount = materialsUsed = 0;
                lastFrameTime = 0.0f;
            }
            uint32_t getTotalVertexCount() { return quadCount * 4; }
            uint32_t getTotalIndexCount() { return quadCount * 6; }
        };

        static Storage rendererData;
        static Statistics rendererStats;


        static void init();
        static void shutdown();

        static void beginScene(const EditorCamera3D& camera);
        static void beginScene(const Scope<Camera>& camera, const glm::mat4& transform);
        static void endScene();
        static void flush();

        // Primitives
        static void drawObject(const glm::mat4& transform,
                               const ObjectRendererComponent& object, int32_t entityID);

        // Stats
        static Statistics& getStats() { return rendererStats; }
        static void resetStats() { rendererStats.reset(); }

    private:
        static void endBatch();
        static void newBatch();
    };
};  // namespace AnEngine

#endif
