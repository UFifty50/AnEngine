#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>


namespace AnEngine {
    class OrthographicCamera;

    enum class CameraType { Perspective = 0, Orthographic = 1 };

    class Camera {
    public:
        virtual ~Camera() = default;

        virtual void setProjection(float left, float right, float bottom, float top) = 0;

        virtual void setPosition(const glm::vec3& position) = 0;
        virtual const glm::vec3& getPosition() const = 0;

        virtual void setRotation(float rotation) = 0;
        virtual float getRotation() const = 0;

        virtual const glm::mat4& getProjectionMatrix() const = 0;
        virtual const glm::mat4& getViewMatrix() const = 0;
        virtual const glm::mat4& getViewProjectionMatrix() const = 0;

        virtual CameraType getType() const = 0;
    };
}  // namespace AnEngine

#endif
