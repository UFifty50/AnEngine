#ifndef OPENGLRENDERAPI_HPP
#define OPENGLRENDERAPI_HPP

#include <glm/glm.hpp>

#include <memory>

#include "Renderer/RenderAPI.hpp"
#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    class OpenGLRenderAPI : public RenderAPI {
    public:
        OpenGLRenderAPI() = default;

        virtual void init() override;
        virtual void setViewport(uint32_t x, uint32_t y, uint32_t width,
                                 uint32_t height) override;

        virtual void clearColour(const glm::vec4& colour) override;
        virtual void clear() override;
        virtual void clearDepth() override;
        virtual void drawIndexed(const Ref<VertexArray>& vertexArray) override;
    };
};  // namespace AnEngine

#endif
