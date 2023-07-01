#include "aepch.hpp"

#include <glad/glad.h>
#include "Platform/OpenGL/OpenGLRenderAPI.hpp"


namespace AnEngine {
    void OpenGLRenderAPI::clearColour(const glm::vec4& colour) {
        glClearColor(colour.r, colour.g, colour.b, colour.a);
    }

    void OpenGLRenderAPI::clear() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGLRenderAPI::clearDepth() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderAPI::drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
        glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
    }
};
