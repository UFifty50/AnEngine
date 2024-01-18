#ifndef RENDERER2D_HPP
#define RENDERER2D_HPP

#include <glm/glm.hpp>

#include <memory>
#include <type_traits>

#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"
#include "Scene/Components.hpp"
#include "Texture/Texture2D.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class Renderer2D {
    public:
        struct QuadVertex {
            glm::vec3 position;
            glm::vec4 colour;
            glm::vec2 texCoord;
            float texIndex;
            float tilingFactor;
            glm::vec4 tint;
            // ShaderUniformVector attributes;

            // Editor Specific
            uint32_t entityID;
        };

        struct Storage {
            static const uint32_t maxQuads = 10'000;
            static const uint32_t maxVertices = maxQuads * 4;
            static const uint32_t maxIndices = maxQuads * 6;
            static const uint32_t maxTextureSlots = 32;  // TODO: Render Capabilities

            Ref<VertexArray> quadVA;
            Ref<VertexBuffer> quadVB;
            Ref<Texture2D> blankTexture;
            ShaderLibrary shaderLibrary;  // Ref<Shader> shader;

            uint32_t quadIndexCount = 0;
            QuadVertex* quadVertexBufferBase = nullptr;
            QuadVertex* quadVertexBufferPtr = nullptr;

            std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
            uint8_t textureSlotIndex = 1;  // 0 = blank texture

            glm::vec4 quadVertexPositions[4];

            bool activeScene = false;
        };

        struct Statistics {
            uint32_t draws = 0;
            uint32_t quadCount = 0;
            float lastFrameTime = 0.0f;

            void reset() {
                draws = quadCount = 0;
                lastFrameTime = 0.0f;
            }
            uint32_t getTotalVertexCount() { return quadCount * 4; }
            uint32_t getTotalIndexCount() { return quadCount * 6; }
        };

        static Storage rendererData;
        static Statistics rendererStats;


        static void init();
        static void shutdown();

        static void beginScene(const EditorCamera& camera);
        static void beginScene(const Camera& camera, const glm::mat4& transform);
        static void endScene();
        static void flush();

        // Primitives
        static void drawSprite(const glm::mat4& transform,
                               const SpriteRendererComponent& sprite, int32_t entityID);

        static void drawQuad(const glm::mat4& transform, const glm::vec4& colour,
                             int32_t entityID = -1);
        static void drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture,
                             int32_t entityID = -1,
                             const ShaderUniformVector& attributes = {});


        /*static void drawQuad(const glm::vec2& position, const glm::vec2& size, float
        rotation, const glm::vec4& colour, const ShaderUniformVector& attributes = {}); static
        void drawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const
        glm::vec4& colour, const ShaderUniformVector& attributes = {});

        static void drawQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                             const Ref<Texture2D>& texture,
                             const ShaderUniformVector& attributes = {});
        static void drawQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                             const Ref<Texture2D>& texture,
                             const ShaderUniformVector& attributes = {});*/

        // Stats
        static Statistics& getStats() { return rendererStats; }
        static void resetStats() { rendererStats.reset(); }

    private:
        static void endBatch();
        static void newBatch();
    };
}  // namespace AnEngine

#endif
