#include "aepch.hpp"

#include "Platform/OpenGL/OpenGLRenderAPI.hpp"

#include <glad/glad.h>


namespace AnEngine {
    void OpenGLRenderAPI::init() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //   glEnable(GL_CULL_FACE);
        //   glCullFace(GL_BACK);

        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRenderAPI::setViewport(uint32_t x, uint32_t y, uint32_t width,
                                      uint32_t height) {
        glViewport(x, y, width, height);
    }

    void OpenGLRenderAPI::clearColour(const glm::vec4& colour) {
        glClearColor(colour.r, colour.g, colour.b, colour.a);
    }

    void OpenGLRenderAPI::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    void OpenGLRenderAPI::drawIndexed(const Ref<VertexArray>& vertexArray,
                                      uint32_t indexCount) {
        vertexArray->bind();

        //   uint32_t count = indexCount ? indexCount :
        //   vertexArray->getIndexBuffer()->getCount();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        //  glBindTexture(GL_TEXTURE_2D, 0);
    }
};  // namespace AnEngine
