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

        virtual void clearColour(const glm::vec4& colour) override;
        virtual void clear() override;
        virtual void clearDepth() override;
        virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
    };
};

#endif
