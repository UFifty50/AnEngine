#include "aepch.hpp"

#include "Renderer/Camera/EditorCamera.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Core/Input.hpp"
#include "Core/KeyCodes.hpp"
#include "Core/MouseCodes.hpp"


namespace AnEngine {
    EditorCamera::EditorCamera(CameraSpec spec) {
        projectionType = spec.type;
        aspectRatio = spec.aspectRatio;

        switch (projectionType) {
            case ProjectionType::Perspective:
                perspectiveSettings = {glm::radians(spec.FOVorSize), spec.nearPlane,
                                       spec.farPlane};
                projectionMatrix =
                    glm::perspective(glm::radians(spec.FOVorSize), spec.aspectRatio,
                                     spec.nearPlane, spec.farPlane);
                break;

            case ProjectionType::Orthographic:
                orthoSettings = {spec.FOVorSize, spec.nearPlane, spec.farPlane};
                orthoSettings.bounds = {-spec.FOVorSize * aspectRatio,
                                        spec.FOVorSize * aspectRatio, -spec.FOVorSize,
                                        spec.FOVorSize};
                projectionMatrix = glm::ortho(
                    -spec.FOVorSize * spec.aspectRatio, spec.FOVorSize * spec.aspectRatio,
                    -spec.FOVorSize, spec.FOVorSize, spec.nearPlane, spec.farPlane);
                break;
        }

        updateViewMatrix();
    }

    /*EditorCamera EditorCamera::makePerspective(float FOVdegrees, float aspectRatio,
                                               float nearPlane, float farPlane) {
        EditorCamera e = EditorCamera();
        e.projectionType = ProjectionType::Perspective;

        e.aspectRatio = aspectRatio;
        e.perspectiveSettings = {glm::radians(FOVdegrees), nearPlane, farPlane};
        e.projectionMatrix =
            glm::perspective(glm::radians(FOVdegrees), aspectRatio, nearPlane, farPlane);
        e.updateViewMatrix();
        return e;
    }

    EditorCamera EditorCamera::makeOrthographic(float orthoSize, float aspectRatio,
                                                float nearPlane, float farPlane) {
        EditorCamera e = EditorCamera();
        e.projectionType = ProjectionType::Orthographic;

        e.aspectRatio = aspectRatio;
        e.orthoSettings = {orthoSize, nearPlane, farPlane};
        e.orthoSettings.bounds = {-orthoSize * aspectRatio, orthoSize * aspectRatio,
                                  -orthoSize, orthoSize};
        e.projectionMatrix = glm::ortho(-orthoSize * aspectRatio, orthoSize * aspectRatio,
                                        -orthoSize, orthoSize, nearPlane, farPlane);
        e.updateViewMatrix();
        return e;
    }*/

    void EditorCamera::changeProjectionType(ProjectionType type) {
        projectionType = type;
        updateProjectionMatrix();
    }

    void EditorCamera::onUpdate(TimeStep deltaTime) {
        const glm::vec2& mouse = {Input::getMouseX(), Input::getMouseY()};
        glm::vec2 delta = (mouse - initialMousePosition) * 0.003f;
        initialMousePosition = mouse;

        if (Input::isMouseButtonPressed(MouseCode::ButtonRight)) {
            mouseRotate(delta);
        } else if (Input::isMouseButtonPressed(MouseCode::ButtonMiddle)) {
            mousePan(delta);
        }

        updateViewMatrix();
    }

    void EditorCamera::onEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<MouseScrolledEvent>(
            BIND_EVENT_FN(EditorCamera::onMouseScrolled));
    }

    void EditorCamera::setViewportSize(float width, float height) {
        viewportWidth = width;
        viewportHeight = height;

        updateProjectionMatrix();
    }

    void EditorCamera::updateProjectionMatrix() {
        aspectRatio = viewportWidth / viewportHeight;

        switch (projectionType) {
            using enum ProjectionType;

            case Perspective:
                projectionMatrix =
                    glm::perspective(perspectiveSettings.FOV, aspectRatio,
                                     perspectiveSettings.near, perspectiveSettings.far);
                break;

            case Orthographic:
                float left = -orthoSettings.size * aspectRatio * 0.5f;
                float right = -left;
                float bottom = -orthoSettings.size * 0.5f;
                float top = -bottom;
                orthoSettings.bounds = {left, right, bottom, top};
                projectionMatrix = glm::ortho(left, right, bottom, top,
                                              orthoSettings.near, orthoSettings.far);
                break;
        }
    }

    void EditorCamera::updateViewMatrix() {
        if (!isPerspective()) yaw = pitch = 0.0f;  // locks the cameras rotation
        position = calculatePosition();
        glm::quat orientation = getOrientation();
        viewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
        viewMatrix = glm::inverse(viewMatrix);
    }

    glm::vec3 EditorCamera::getDirection(Direction direction) const {
        switch (direction) {
            using enum Direction;

            case UP:
                return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
            case DOWN:
                return glm::rotate(getOrientation(), glm::vec3(0.0f, -1.0f, 0.0f));
            case RIGHT:
                return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
            case LEFT:
                return glm::rotate(getOrientation(), glm::vec3(-1.0f, 0.0f, 0.0f));
            case FORWARD:
                return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
            case BACK:
                return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, 1.0f));
        }

        return glm::vec3(0.0f);
    }

    glm::quat EditorCamera::getOrientation() const {
        return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
    }

    bool EditorCamera::onMouseScrolled(MouseScrolledEvent& e) {
        float delta = e.getYOffset() * 0.1f;
        mouseZoom(delta);
        updateViewMatrix();
        return false;
    }

    void EditorCamera::mousePan(const glm::vec2& delta) {
        auto [xSpeed, ySpeed] = getPanSpeed();
        focalPoint += -getDirection(Direction::RIGHT) * delta.x * xSpeed * distance;
        focalPoint += getDirection(Direction::UP) * delta.y * ySpeed * distance;
    }

    void EditorCamera::mouseRotate(const glm::vec2& delta) {
        float yawSign = getDirection(Direction::UP).y < 0 ? -1.0f : 1.0f;
        yaw += yawSign * delta.x * getRotationSpeed();
        pitch += delta.y * getRotationSpeed();
    }

    void EditorCamera::mouseZoom(float delta) {
        distance -= delta * getZoomSpeed();

        if (distance < 1.0f) {
            focalPoint += getDirection(Direction::FORWARD);
            distance = 1.0f;
        }
    }

    glm::vec3 EditorCamera::calculatePosition() const {
        return focalPoint - getDirection(Direction::FORWARD) * distance;
    }

    std::pair<float, float> EditorCamera::getPanSpeed() const {
        float x = std::min(viewportWidth / 1000.0f, 2.4f);
        float y = std::min(viewportHeight / 1000.0f, 2.4f);

        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return {xFactor, yFactor};
    }

    float EditorCamera::getRotationSpeed() const { return 0.8f; }

    float EditorCamera::getZoomSpeed() const {
        float dist = std::max(distance * 0.2f, 0.0f);
        return std::min(dist * dist, 100.0f);
    }
};  // namespace AnEngine
