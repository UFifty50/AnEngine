#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>


namespace AnEngine {
    enum class ProjectionType { Perspective = 0, Orthographic = 1 };

    class BaseCamera {
    public:
        virtual ~BaseCamera() = default;

        virtual void setPosition(const glm::vec3& position) = 0;
        virtual const glm::vec3& getPosition() const = 0;

        virtual void setRotation(float rotation) = 0;
        virtual float getRotation() const = 0;

        virtual const glm::mat4& getProjectionMatrix() const = 0;
        virtual const glm::mat4& getViewMatrix() const = 0;
        virtual const glm::mat4& getViewProjectionMatrix() const = 0;

        virtual ProjectionType getType() const = 0;
    };

    class ComponentCamera {
    public:
        ComponentCamera() = default;
        ComponentCamera(const glm::mat4& projection) : projectionMatrix(projection) {}
        virtual ~ComponentCamera() = default;

        const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
        const ProjectionType getProjectionType() const { return projectionType; }

    protected:
        ProjectionType projectionType = ProjectionType::Orthographic;
        glm::mat4 projectionMatrix{1.0f};
    };
}  // namespace AnEngine

#endif
