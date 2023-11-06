#ifndef EDITORCAMERA_HPP
#define EDITORCAMERA_HPP

#include <glm/glm.hpp>

#include "Events/Event.hpp"
#include "Events/MouseEvent.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class EditorCamera : public Camera {
    public:
        enum class Direction { UP, DOWN, LEFT, RIGHT, FORWARD, BACK };

        EditorCamera() = default;

        static EditorCamera makePerspective(float FOVdegrees, float aspectRatio,
                                            float nearPlane, float farPlane);
        static EditorCamera makeOrthographic(float orthoSize, float aspectRatio,
                                             float nearPlane, float farPlane);

        void changeProjectionType(ProjectionType type);

        void onUpdate(TimeStep deltaTime);
        void onEvent(Event& e);

        bool isPerspective() const {
            return projectionType == ProjectionType::Perspective;
        }

        float getDistance() const { return distance; }
        void setDistance(float distance) { this->distance = distance; }

        void setViewportSize(float width, float height);

        const glm::mat4& getViewMatrix() const { return viewMatrix; }
        glm::mat4 getViewProjectionMatrix() const {
            return projectionMatrix * viewMatrix;
        }

        glm::vec3 getDirection(Direction direction) const;
        const glm::vec3& getPosition() const { return position; }
        glm::quat getOrientation() const;

        float getPitch() const { return pitch; }
        float yetYaw() const { return yaw; }

    private:
        void updateProjectionMatrix();
        void updateViewMatrix();

        bool onMouseScrolled(MouseScrolledEvent& e);

        void mousePan(const glm::vec2& delta);
        void mouseRotate(const glm::vec2& delta);
        void mouseZoom(float delta);

        glm::vec3 calculatePosition() const;

        std::pair<float, float> getPanSpeed() const;
        float getRotationSpeed() const;
        float getZoomSpeed() const;


        glm::mat4 viewMatrix{0.0f};
        glm::vec3 position{0.0f};
        glm::vec3 focalPoint{0.0f};

        glm::vec2 initialMousePosition{0.0f};

        float distance = 10.0f;
        float pitch = 0.0f;
        float yaw = 0.0f;

        float viewportWidth = 1280.0f;
        float viewportHeight = 720.0f;

        float aspectRatio = 1.778f;

        struct {
            float FOV = glm::radians(45.0f);
            float near = 0.01f;
            float far = 1000.0f;
        } perspectiveSettings;

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
};  // namespace AnEngine

#endif
