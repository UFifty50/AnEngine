#include "aepch.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Camera/SceneCamera.hpp"

#include "Core/Core.hpp"
#include "Core/Log.hpp"


namespace AnEngine {
    SceneCamera::SceneCamera() { recalculateProjection(); }
    SceneCamera::SceneCamera(CameraSpec3D spec) {
        AE_PROFILE_FUNCTION()

        projectionType = spec.type;
        isPerspective = spec.type == ProjectionType::Perspective;
        aspectRatio = spec.aspectRatio;

        switch (projectionType) {
            case ProjectionType::Perspective:
                perspectiveSettings = {glm::radians(spec.FOVorSize), spec.nearPlane,
                                       spec.farPlane};
                projectionMatrix =
                    glm::perspective(glm::radians(spec.FOVorSize), spec.aspectRatio,
                                     spec.nearPlane, spec.farPlane);
                break;

            case ProjectionType::Orthographic:
                orthoSettings = {spec.FOVorSize, spec.nearPlane, spec.farPlane};
                orthoSettings.bounds = {-spec.FOVorSize * aspectRatio,
                                        spec.FOVorSize * aspectRatio, -spec.FOVorSize,
                                        spec.FOVorSize};
                projectionMatrix = glm::ortho(
                    -spec.FOVorSize * spec.aspectRatio, spec.FOVorSize * spec.aspectRatio,
                    -spec.FOVorSize, spec.FOVorSize, spec.nearPlane, spec.farPlane);
                break;
        }
    }

    void SceneCamera::updateSpec(CameraSpec3D::Feild feild, float value) {
        AE_PROFILE_FUNCTION()

        switch (feild) {
            using enum CameraSpec3D::Feild;

            case Type:
                projectionType = ProjectionType::fromFloat(value);
                isPerspective = projectionType == ProjectionType::Perspective;
                break;

            case FOVorSize:
                if (isPerspective)
                    perspectiveSettings.FOV = glm::radians(value);
                else
                    orthoSettings.size = value;

            case AspectRatio:
                aspectRatio = value;
                break;

            case NearPlane:
                if (isPerspective)
                    perspectiveSettings.near = value;
                else
                    orthoSettings.near = value;
                break;

            case FarPlane:
                if (isPerspective)
                    perspectiveSettings.far = value;
                else
                    orthoSettings.far = value;
                break;

            default:
                AE_CORE_ASSERT(false, "Invalid camera spec feild!");
                return;
        }

        recalculateProjection();
    }

    void SceneCamera::setViewportSize(uint32_t width, uint32_t height) {
        AE_PROFILE_FUNCTION()

        AE_CORE_ASSERT(width > 0 && height > 0, "Invalid viewport width or height!");

        aspectRatio = (float)width / (float)height;
        recalculateProjection();
    }

    void SceneCamera::recalculateProjection() {
        AE_PROFILE_FUNCTION()

        if (projectionType == ProjectionType::Orthographic) {
            float left = -orthoSettings.size * aspectRatio * 0.5f;
            float right = -left;
            float top = orthoSettings.size * 0.5f;
            float bottom = -top;
            orthoSettings.bounds = {left, right, bottom, top};
            projectionMatrix =
                glm::ortho(left, right, bottom, top, orthoSettings.near, orthoSettings.far);
        } else {
            projectionMatrix =
                glm::perspective(perspectiveSettings.FOV, aspectRatio,
                                 perspectiveSettings.near, perspectiveSettings.far);
        }
    }
}  // namespace AnEngine
