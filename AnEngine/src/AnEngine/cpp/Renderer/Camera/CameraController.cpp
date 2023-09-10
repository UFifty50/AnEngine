#include "aepch.hpp"

#include "Renderer/Camera/CameraController.hpp"

#include "Core/Input.hpp"
#include "Core/KeyCodes.hpp"
#include "Renderer/Camera/OrthographicCamera.hpp"
#include "Renderer/Camera/PerspectiveCamera.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    CameraController::CameraController(float aspectRatio, float fov, bool rotationEnabled,
                                       bool isOrthographic)
        : aspectRatio(aspectRatio),
          fov(fov),
          rotation(rotationEnabled),
          orthographic(isOrthographic) {
        if (isOrthographic) {
            camera = MakeRef<OrthographicCamera>(-aspectRatio * zoom, aspectRatio * zoom,
                                                 -zoom, zoom);
        } else {
            AE_CORE_ASSERT(false, "Perspective camera not implemented yet!")
            // camera = MakeRef<PerspectiveCamera>(fov, aspectRatio, near, far);
        }
    }

    void CameraController::onUpdate(TimeStep deltaTime) {
        if (Input::isKeyPressed(AE_KEY_A)) {
            cameraPosition.x -= cameraTranslationSpeed * (zoom / 2) * deltaTime;
        } else if (Input::isKeyPressed(AE_KEY_D)) {
            cameraPosition.x += cameraTranslationSpeed * (zoom / 2) * deltaTime;
        }

        if (Input::isKeyPressed(AE_KEY_W)) {
            cameraPosition.y += cameraTranslationSpeed * (zoom / 2) * deltaTime;
        } else if (Input::isKeyPressed(AE_KEY_S)) {
            cameraPosition.y -= cameraTranslationSpeed * (zoom / 2) * deltaTime;
        }

        if (rotation) {
            if (Input::isKeyPressed(AE_KEY_Q)) {
                cameraRotation += cameraRotationSpeed * deltaTime;
            } else if (Input::isKeyPressed(AE_KEY_E)) {
                cameraRotation -= cameraRotationSpeed * deltaTime;
            }

            camera->setRotation(cameraRotation);
        }

        camera->setPosition(cameraPosition);
    }

    void CameraController::onEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<MouseScrolledEvent>(
            BIND_EVENT_FN(CameraController::onMouseScrolled));
        dispatcher.dispatch<WindowResizeEvent>(
            BIND_EVENT_FN(CameraController::onWindowResized));
    }

    void CameraController::onResize(float width, float height) {
        aspectRatio = width / height;
        camera->setProjection(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom);
    }

    float zoomAlgorithm(float currentZoom, float distanceScrolled, float minZoom,
                        float maxZoom) {
        // Define the scaling factors to control the speed of zooming
        float zoomInFactor = 0.2f;   // Controls how fast the zoom increases
        float zoomOutFactor = 0.2f;  // Controls how fast the zoom decreases

        // Calculate the new zoom level based on the distance scrolled
        float newZoom;

        if (distanceScrolled < 0) {
            // Zoom in
            newZoom = currentZoom / (1 + distanceScrolled * zoomInFactor);
        } else {
            // Zoom out
            newZoom = currentZoom * (1 - distanceScrolled * zoomOutFactor);
        }

        // Ensure the new zoom level stays within the defined range [minZoom, maxZoom]
        newZoom = std::clamp(newZoom, minZoom, maxZoom);

        return newZoom;
    }

    bool CameraController::onMouseScrolled(MouseScrolledEvent& scrollEvent) {
        zoom = zoomAlgorithm(zoom, scrollEvent.getYOffset(), 0.1f, 15.0f);
        camera->setProjection(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom);

        return false;
    }

    bool CameraController::onWindowResized(WindowResizeEvent& resizeEvent) {
        onResize((float)resizeEvent.getWidth(), (float)resizeEvent.getHeight());

        return false;
    }
};  // namespace AnEngine
