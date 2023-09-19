#include "aepch.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene/SceneCamera.hpp"


namespace AnEngine {
    SceneCamera::SceneCamera() { recalculateProjection(); }

    void SceneCamera::setOrthographic(float size, float nearPlane, float farPlane) {
        orthoSize = size;
        orthoNear = nearPlane;
        orthoFar = farPlane;

        recalculateProjection();
    }

    void SceneCamera::setViewportSize(uint32_t width, uint32_t height) {
        aspectRatio = (float)width / (float)height;
        recalculateProjection();
    }

    void SceneCamera::recalculateProjection() {
        float left = -orthoSize * aspectRatio * 0.5;
        float right = -left;
        float top = orthoSize * 0.5;
        float bottom = -top;
        projectionMatrix = glm::ortho(left, right, bottom, top, orthoNear, orthoFar);
    }
}  // namespace AnEngine
