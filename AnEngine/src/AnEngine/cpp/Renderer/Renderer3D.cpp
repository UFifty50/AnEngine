#include "aepch.hpp"

#include "Renderer/Renderer3D.hpp"

#include "Renderer/Renderer.hpp"


namespace AnEngine {
    Renderer3D::Storage Renderer3D::rendererData;
    Renderer3D::Statistics Renderer3D::rendererStats;

    void Renderer3D::init() {}

    void Renderer3D::shutdown() {}

    void Renderer3D::beginScene(const EditorCamera3D& camera) {}

    void Renderer3D::beginScene(const Scope<Camera>& camera, const glm::mat4& transform) {}

    void Renderer3D::endScene() {}

    void Renderer3D::newBatch() {}

    void Renderer3D::endBatch() {}

    void Renderer3D::flush() {}

    void Renderer3D::drawObject(const glm::mat4& transform,
                                const ObjectRendererComponent& object, int32_t entityID) {}
};  // namespace AnEngine
