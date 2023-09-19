#ifndef SCENECAMERA_HPP
#define SCENECAMERA_HPP

#include "Renderer/Camera/Camera.hpp"


namespace AnEngine {
    class SceneCamera : public ComponentCamera {
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void setOrthographic(float size, float nearPlane, float farPlane);
        void setViewportSize(uint32_t width, uint32_t height);

        float getOrthographicSize() const { return orthoSize; }
        void setOrthographicSize(float size) {
            orthoSize = size;
            recalculateProjection();
        }

    private:
        float orthoSize = 10.0f;
        float orthoNear = -1.0f;
        float orthoFar = 1.0f;

        float aspectRatio = 1.0f;

        void recalculateProjection();
    };
}  // namespace AnEngine

#endif
