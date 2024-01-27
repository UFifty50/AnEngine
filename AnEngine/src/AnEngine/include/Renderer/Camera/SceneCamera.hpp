#ifndef SCENECAMERA_HPP
#define SCENECAMERA_HPP

#include <any>

#include "Core/Core.hpp"
#include "Renderer/Camera/Camera.hpp"


namespace AnEngine {
    class SceneCamera : public Camera {
    public:
        SceneCamera();
        SceneCamera(CameraSpec3D spec);
        virtual ~SceneCamera() = default;

        virtual bool is3D() const override { return isPerspective; }

        virtual void changeProjectionType(ProjectionType type) override {
            projectionType = type;
            isPerspective = projectionType == ProjectionType::Perspective;
            recalculateProjection();
        }

        void updateSpec(CameraSpec3D::Feild feild, float value);

        void setViewportSize(uint32_t width, uint32_t height);

        float getAspectRatio() const { return aspectRatio; }

        const glm::mat4& getViewMatrix() const override {
            AE_CORE_ASSERT(false, "Not implemented");
            return glm::mat4(1.0f);
        }

        float getNearPlane() const {
            return isPerspective ? perspectiveSettings.near : orthoSettings.near;
        }

        float getFarPlane() const {
            return isPerspective ? perspectiveSettings.far : orthoSettings.far;
        }

        float getFOV() const { return perspectiveSettings.FOV; }
        float getOrthoSize() const { return orthoSettings.size; }

    private:
        void recalculateProjection();


        bool isPerspective;
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
