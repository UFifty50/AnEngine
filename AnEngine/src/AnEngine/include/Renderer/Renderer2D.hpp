#ifndef RENDERER2D_HPP
#define RENDERER2D_HPP

#include <glm/glm.hpp>

#include <memory>
#include <type_traits>

#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"
#include "Texture/Texture2D.hpp"


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
            float rotation;
            // ShaderUniformVector attributes;
        };

        struct Storage {
            const uint32_t maxQuads = 10'000;
            const uint32_t maxVertices = maxQuads * 4;
            const uint32_t maxIndices = maxQuads * 6;
            static const uint32_t maxTextureSlots = 32;  // TODO: Render Capabilities

            Ref<VertexArray> quadVA;
            Ref<VertexBuffer> quadVB;
            Ref<Texture2D> blankTexture;
            ShaderLibrary shaderLibrary;  // Ref<Shader> shader;

            uint32_t quadIndexCount = 0;
            QuadVertex* quadVertexBufferBase = nullptr;
            QuadVertex* quadVertexBufferPtr = nullptr;

            std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
            uint32_t textureSlotIndex = 1;  // 0 = blank texture
        };

        static Storage rendererData;


        static void init();
        static void shutdown();

        static void beginScene(const Ref<Camera>& camera);
        static void endScene();
        static void flush();

        // Primitives
        static void drawQuad(const glm::vec2& position, const glm::vec2& size,
                             float rotation, const glm::vec4& colour,
                             const AnEngine::ShaderUniformVector& attributes = {});
        static void drawQuad(const glm::vec3& position, const glm::vec2& size,
                             float rotation, const glm::vec4& colour,
                             const AnEngine::ShaderUniformVector& attributes = {});

        static void drawQuad(const glm::vec2& position, const glm::vec2& size,
                             float rotation, const Ref<Texture2D>& texture,
                             const AnEngine::ShaderUniformVector& attributes = {});
        static void drawQuad(const glm::vec3& position, const glm::vec2& size,
                             float rotation, const Ref<Texture2D>& texture,
                             const AnEngine::ShaderUniformVector& attributes = {});
    };
}  // namespace AnEngine

#endif
