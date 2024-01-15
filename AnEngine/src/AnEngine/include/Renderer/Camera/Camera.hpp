#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>


namespace AnEngine {
    enum class ProjectionType { Perspective = 0, Orthographic = 1 };

    class Camera {
    public:
        Camera() = default;
        Camera(const glm::mat4& projection) : projectionMatrix(projection) {}
        virtual ~Camera() = default;

        const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
        const ProjectionType getProjectionType() const { return projectionType; }

    protected:
        ProjectionType projectionType = ProjectionType::Perspective;
        glm::mat4 projectionMatrix{1.0f};
    };
}  // namespace AnEngine

#endif
