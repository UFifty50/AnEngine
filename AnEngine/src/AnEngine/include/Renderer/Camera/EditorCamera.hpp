#ifndef EDITORCAMERA_HPP
#define EDITORCAMERA_HPP

#include <glm/glm.hpp>

#include "Events/Event.hpp"
#include "Events/MouseEvent.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class EditorCamera3D : public EditorCamera {
    public:
        EditorCamera3D(CameraSpec3D spec);

        virtual bool is3D() const override { return true; }
        virtual void changeProjectionType(ProjectionType type) override;

        virtual void onUpdate(TimeStep deltaTime) override;
        virtual void onEvent(Event& e) override;

        virtual bool isPerspective() const override {
            return projectionType == ProjectionType::Perspective;
        }

        glm::vec3 getDirection(Direction direction) const;
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

        float pitch = 0.0f;
        float yaw = 0.0f;

        struct {
            float FOV = glm::radians(45.0f);
            float near = 0.01f;
            float far = 1000.0f;
        } perspectiveSettings;
    };


    class EditorCamera2D : public EditorCamera {
    public:
        EditorCamera2D(CameraSpec2D spec);

        virtual constexpr bool is3D() const override { return false; }
        virtual constexpr bool isPerspective() const override { return false; }

        virtual void changeProjectionType(ProjectionType type) override {
            AE_CORE_WARN("Cannot change 2D projection type!");
        }

        virtual void onUpdate(TimeStep deltaTime) override;
        virtual void onEvent(Event& e) override;

        glm::vec3 getDirection(Direction direction) const;
        glm::quat getOrientation() const;

    private:
        void updateProjectionMatrix();
        void updateViewMatrix();

        bool onMouseScrolled(MouseScrolledEvent& e);

        void mousePan(const glm::vec2& delta);
        void mouseZoom(float delta);

        glm::vec3 calculatePosition() const;

        std::pair<float, float> getPanSpeed() const;
        float getZoomSpeed() const;
    };
};  // namespace AnEngine

#endif
