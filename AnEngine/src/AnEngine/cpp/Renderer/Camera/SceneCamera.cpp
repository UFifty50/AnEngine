#include "aepch.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Camera/SceneCamera.hpp"

#include "Core/Log.hpp"


namespace AnEngine {
    SceneCamera::SceneCamera() { recalculateProjection(); }

    void SceneCamera::setOrthographic(float size, float nearPlane, float farPlane) {
        projectionType = ProjectionType::Orthographic;

        orthoSettings.size = size;
        orthoSettings.near = nearPlane;
        orthoSettings.far = farPlane;

        recalculateProjection();
    }

    void SceneCamera::setPerspective(float FOV, float nearPlane, float farPlane) {
        projectionType = ProjectionType::Perspective;

        perspectiveSettings.FOV = FOV;
        perspectiveSettings.near = nearPlane;
        perspectiveSettings.far = farPlane;

        recalculateProjection();
    }

    void SceneCamera::setViewportSize(uint32_t width, uint32_t height) {
        aspectRatio = (float)width / (float)height;

        recalculateProjection();
    }

    void SceneCamera::recalculateProjection() {
        if (projectionType == ProjectionType::Orthographic) {
            float left = -orthoSettings.size * aspectRatio * 0.5f;
            float right = -left;
            float top = orthoSettings.size * 0.5f;
            float bottom = -top;
            orthoSettings.bounds = {left, right, bottom, top};
            projectionMatrix = glm::ortho(left, right, bottom, top, orthoSettings.near,
                                          orthoSettings.far);
        } else {
            float top =
                glm::tan(perspectiveSettings.FOV * 0.5f) * perspectiveSettings.near;
            float bottom = -top;
            float right = top * aspectRatio;
            float left = -right;

            //    perspectiveSettings.bounds = {left, right, bottom, top};
            projectionMatrix =
                glm::perspective(perspectiveSettings.FOV, aspectRatio,
                                 perspectiveSettings.near, perspectiveSettings.far);
        }
    }
}  // namespace AnEngine
