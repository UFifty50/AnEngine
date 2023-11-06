#ifndef SCENECAMERA_HPP
#define SCENECAMERA_HPP

#include "Renderer/Camera/Camera.hpp"


namespace AnEngine {
    class SceneCamera : public Camera {
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void setOrthographic(float size, float nearPlane, float farPlane);
        void setPerspective(float FOV, float nearPlane, float farPlane);
        void setViewportSize(uint32_t width, uint32_t height);

        float getOrthographicSize() const { return orthoSettings.size; }
        float getOrthographicNear() const { return orthoSettings.near; }
        float getOrthographicFar() const { return orthoSettings.far; }

        float getPerspectiveFOV() const { return perspectiveSettings.FOV; }
        float getPerspectiveNear() const { return perspectiveSettings.near; }
        float getPerspectiveFar() const { return perspectiveSettings.far; }

        void setOrthographicSize(float size) {
            orthoSettings.size = size;
            recalculateProjection();
        }

        void setOrthographicNear(float nearPlane) {
            orthoSettings.near = nearPlane;
            recalculateProjection();
        }

        void setOrthographicFar(float farPlane) {
            orthoSettings.far = farPlane;
            recalculateProjection();
        }

        void setPerspectiveFOV(float fov) {
            perspectiveSettings.FOV = fov;
            recalculateProjection();
        }

        void setPerspectiveNear(float nearPlane) {
            perspectiveSettings.near = nearPlane;
            recalculateProjection();
        }

        void setPerspectiveFar(float farPlane) {
            perspectiveSettings.far = farPlane;
            recalculateProjection();
        }

        decltype(auto) getOrthographicConfig() const { return orthoSettings; }
        decltype(auto) getPerspectiveConfig() const { return perspectiveSettings; }

        ProjectionType getType() const { return projectionType; }
        void setType(ProjectionType type) {
            projectionType = type;
            isPerspective = projectionType == ProjectionType::Perspective;
            isOrthographic = !isPerspective;
            recalculateProjection();
        }

    private:
        void recalculateProjection();

        bool isPerspective = projectionType == ProjectionType::Perspective;
        bool isOrthographic = !isPerspective;

        float aspectRatio = 1.0f;

        struct {
            float size = 10.0f;
            float near = -1.0f;
            float far = 1.0f;

            struct {
                float left;
                float right;
                float bottom;
                float top;

                float getWidth() { return right - left; }
                float getHeight() { return top - bottom; }
            } bounds;
        } orthoSettings;

        struct {
            float FOV = glm::radians(45.0f);
            float near = 0.01f;
            float far = 1000.0f;
        } perspectiveSettings;

        friend class SceneSerialiser;
    };
}  // namespace AnEngine

#endif
