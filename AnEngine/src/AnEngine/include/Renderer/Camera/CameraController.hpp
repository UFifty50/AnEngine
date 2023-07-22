#ifndef CAMERACONTROLLER_HPP
#define CAMERACONTROLLER_HPP

#include "Events/ApplicationEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Renderer/Camera/OrthographicCamera.hpp"
#include "Renderer/Camera/PerspectiveCamera.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class CameraController {
    public:
        CameraController(float aspectRatio, float fov, bool rotationEnabled = false,
                         bool isOrthographic = false);

        void onUpdate(TimeStep deltaTime);
        void onEvent(Event& event);

        Ref<Camera> getCamera() { return camera; }
        const Ref<Camera> getCamera() const { return camera; }

        void setZoom(float level) { zoom = level; }
        float getZoom() const { return zoom; }

    private:
        float aspectRatio;
        float fov;
        float zoom = 1.0f;
        bool orthographic;
        bool rotation;

        Ref<Camera> camera;

        glm::vec3 cameraPosition = {0.0f, 0.0f, 0.0f};
        float cameraRotation = 0.0f;

        float cameraTranslationSpeed = 1.0f;
        float cameraRotationSpeed = 10.0f;

        bool onMouseScrolled(MouseScrolledEvent& scrollEvent);
        bool onWindowResized(WindowResizeEvent& resizeEvent);
    };
};  // namespace AnEngine

#endif
