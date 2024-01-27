#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

#include "Core/Core.hpp"
#include "Events/Event.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class ProjectionType {
    public:
        enum T : uint8_t { Perspective = 0, Orthographic = 1 };

        ProjectionType() = default;
        constexpr ProjectionType(T type) : type(type) {}

        constexpr operator T() const { return type; }

        constexpr std::string toString() const {
            switch (type) {
                case Perspective:
                    return "Perspective";
                case Orthographic:
                    return "Orthographic";
                default:
                    return "Unknown";
            }
        }

        constexpr static ProjectionType fromFloat(int value) { return (T)value; }

        constexpr static ProjectionType fromString(const std::string& string) {
            if (string == "Perspective") {
                return ProjectionType(Perspective);
            } else if (string == "Orthographic") {
                return ProjectionType(Orthographic);
            } else {
                AE_CORE_ERROR("Unknown projection type!");
                return ProjectionType(Perspective);
            }
        }

    private:
        T type;
    };


    struct CameraSpec2D {
        float size;
        float aspectRatio;
        float nearPlane;
        float farPlane;

        enum class Feild { Size = 1, AspectRatio = 2, NearPlane = 3, FarPlane = 4 };

        CameraSpec2D(float size, float aspectRatio = 1.778f, float nearPlane = -1.0f,
                     float farPlane = 1.0f)
            : size(size), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane) {}
    };

    struct CameraSpec3D {
        ProjectionType type;
        float FOVorSize;
        float aspectRatio;
        float nearPlane;
        float farPlane;

        enum class Feild {
            Type = 0,
            FOVorSize = 1,
            AspectRatio = 2,
            NearPlane = 3,
            FarPlane = 4
        };

        CameraSpec3D(ProjectionType type, float FOVorSize, float aspectRatio = 1.778f,
                     float nearPlane = 0.1f, float farPlane = 1000.0f)
            : type(type),
              FOVorSize(FOVorSize),
              aspectRatio(aspectRatio),
              nearPlane(nearPlane),
              farPlane(farPlane) {}

        CameraSpec3D(const CameraSpec2D& spec)
            : type(ProjectionType::Orthographic),
              FOVorSize(spec.size),
              aspectRatio(spec.aspectRatio),
              nearPlane(spec.nearPlane),
              farPlane(spec.farPlane) {}
    };


    class Camera {
    public:
        Camera() = default;
        Camera(const glm::mat4& projection) : projectionMatrix(projection) {}
        virtual ~Camera() = default;

        virtual bool is3D() const = 0;
        virtual void changeProjectionType(ProjectionType type) = 0;
        virtual const glm::mat4& getViewMatrix() const = 0;

        const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
        const ProjectionType getProjectionType() const { return projectionType; }

    protected:
        ProjectionType projectionType = ProjectionType::Perspective;
        glm::mat4 projectionMatrix{1.0f};
    };

    class EditorCamera : public Camera {
    public:
        enum class Direction { UP, DOWN, LEFT, RIGHT, FORWARD, BACK };

        virtual void onUpdate(TimeStep deltaTime) = 0;
        virtual void onEvent(Event& e) = 0;
        virtual void updateProjectionMatrix() = 0;
        virtual void updateViewMatrix() = 0;
        virtual bool isPerspective() const = 0;


        const glm::vec3& getPosition() const { return position; }
        float getDistance() const { return distance; }
        void setDistance(float distance) { this->distance = distance; }

        virtual const glm::mat4& getViewMatrix() const override { return viewMatrix; }
        glm::mat4 getViewProjectionMatrix() const { return projectionMatrix * viewMatrix; }

        void setViewportSize(float width, float height) {
            viewportWidth = width;
            viewportHeight = height;

            updateProjectionMatrix();
        }

    protected:
        glm::mat4 viewMatrix{0.0f};
        glm::vec3 position{0.0f};
        glm::vec3 focalPoint{0.0f};

        glm::vec2 initialMousePosition{0.0f};

        float distance = 10.0f;

        float viewportWidth = 1280.0f;
        float viewportHeight = 720.0f;

        float aspectRatio = 1.778f;

        struct {
            float size = 10.0f;
            float near = -1.0f;
            float far = 10.0f;

            struct {
                float left;
                float right;
                float bottom;
                float top;

                float getWidth() { return right - left; }
                float getHeight() { return top - bottom; }
            } bounds;
        } orthoSettings;
    };

}  // namespace AnEngine

#endif
