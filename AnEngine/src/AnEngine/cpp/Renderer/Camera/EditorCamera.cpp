#include "aepch.hpp"

#include "Renderer/Camera/EditorCamera.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Core/Input.hpp"
#include "Core/KeyCodes.hpp"
#include "Core/MouseCodes.hpp"


namespace AnEngine {
    EditorCamera3D::EditorCamera3D(CameraSpec3D spec) {
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

    void EditorCamera3D::changeProjectionType(ProjectionType type) {
        projectionType = type;
        updateProjectionMatrix();
    }

    void EditorCamera3D::onUpdate(TimeStep deltaTime) {
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

    void EditorCamera3D::onEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<MouseScrolledEvent>(
            BIND_EVENT_FN(EditorCamera3D::onMouseScrolled));
    }

    void EditorCamera3D::updateProjectionMatrix() {
        aspectRatio = viewportWidth / viewportHeight;

        switch (projectionType) {
            using enum ProjectionType::T;

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
                projectionMatrix = glm::ortho(left, right, bottom, top, orthoSettings.near,
                                              orthoSettings.far);
                break;
        }
    }

    void EditorCamera3D::updateViewMatrix() {
        if (!isPerspective()) yaw = pitch = 0.0f;  // locks the cameras rotation
        position = calculatePosition();
        glm::quat orientation = getOrientation();
        viewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
        viewMatrix = glm::inverse(viewMatrix);
    }

    glm::vec3 EditorCamera3D::getDirection(Direction direction) const {
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

    glm::quat EditorCamera3D::getOrientation() const {
        return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
    }

    bool EditorCamera3D::onMouseScrolled(MouseScrolledEvent& e) {
        float delta = e.getYOffset() * 0.1f;
        mouseZoom(delta);
        updateViewMatrix();
        return false;
    }

    void EditorCamera3D::mousePan(const glm::vec2& delta) {
        auto [xSpeed, ySpeed] = getPanSpeed();
        focalPoint += -getDirection(Direction::RIGHT) * delta.x * xSpeed * distance;
        focalPoint += getDirection(Direction::UP) * delta.y * ySpeed * distance;
    }

    void EditorCamera3D::mouseRotate(const glm::vec2& delta) {
        float yawSign = getDirection(Direction::UP).y < 0 ? -1.0f : 1.0f;
        yaw += yawSign * delta.x * getRotationSpeed();
        pitch += delta.y * getRotationSpeed();
    }

    void EditorCamera3D::mouseZoom(float delta) {
        distance -= delta * getZoomSpeed();

        if (distance < 1.0f) {
            focalPoint += getDirection(Direction::FORWARD);
            distance = 1.0f;
        }
    }

    glm::vec3 EditorCamera3D::calculatePosition() const {
        return focalPoint - getDirection(Direction::FORWARD) * distance;
    }

    std::pair<float, float> EditorCamera3D::getPanSpeed() const {
        float x = std::min(viewportWidth / 1000.0f, 2.4f);
        float y = std::min(viewportHeight / 1000.0f, 2.4f);

        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return {xFactor, yFactor};
    }

    float EditorCamera3D::getRotationSpeed() const { return 0.8f; }

    float EditorCamera3D::getZoomSpeed() const {
        float dist = std::max(distance * 0.2f, 0.0f);
        return std::min(dist * dist, 100.0f);
    }


    ////////////////////////////////////////////////////////////////////
    ///\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\///
    ///||||||||||||||||||||||| EditorCamera2D |||||||||||||||||||||||///
    ///\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\///
    ////////////////////////////////////////////////////////////////////


    EditorCamera2D::EditorCamera2D(CameraSpec2D spec) {
        projectionType = ProjectionType::Orthographic;
        aspectRatio = spec.aspectRatio;

        orthoSettings = {spec.size, spec.nearPlane, spec.farPlane};
        orthoSettings.bounds = {-spec.size * aspectRatio, spec.size * aspectRatio, -spec.size,
                                spec.size};
        projectionMatrix =
            glm::ortho(-spec.size * spec.aspectRatio, spec.size * spec.aspectRatio, -spec.size,
                       spec.size, spec.nearPlane, spec.farPlane);


        updateViewMatrix();
    }

    void EditorCamera2D::onUpdate(TimeStep deltaTime) {
        const glm::vec2& mouse = {Input::getMouseX(), Input::getMouseY()};
        glm::vec2 delta = (mouse - initialMousePosition) * 0.003f;
        initialMousePosition = mouse;

        if (Input::isMouseButtonPressed(MouseCode::ButtonMiddle)) {
            mousePan(delta);
        }

        updateViewMatrix();
    }

    void EditorCamera2D::onEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<MouseScrolledEvent>(
            BIND_EVENT_FN(EditorCamera2D::onMouseScrolled));
    }

    void EditorCamera2D::updateProjectionMatrix() {
        aspectRatio = viewportWidth / viewportHeight;

        float left = -orthoSettings.size * aspectRatio * 0.5f;
        float right = -left;
        float bottom = -orthoSettings.size * 0.5f;
        float top = -bottom;
        orthoSettings.bounds = {left, right, bottom, top};
        projectionMatrix =
            glm::ortho(left, right, bottom, top, orthoSettings.near, orthoSettings.far);


        AE_CORE_DEBUG("orthoSettings.size: {0}", orthoSettings.size);
    }

    void EditorCamera2D::updateViewMatrix() {
        position = calculatePosition();
        viewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::mat4(1.0f);
        viewMatrix = glm::inverse(viewMatrix);
    }

    bool EditorCamera2D::onMouseScrolled(MouseScrolledEvent& e) {
        float delta = e.getYOffset() * 0.1f;
        mouseZoom(delta);
        updateViewMatrix();
        return false;
    }

    void EditorCamera2D::mousePan(const glm::vec2& delta) {
        auto [xSpeed, ySpeed] = getPanSpeed();
        focalPoint.x += -delta.x * xSpeed * orthoSettings.size;
        focalPoint.y += delta.y * ySpeed * orthoSettings.size;
    }

    void EditorCamera2D::mouseZoom(float delta) {
        orthoSettings.size -= delta * getZoomSpeed();

        if (orthoSettings.size < 1.0f) {
            orthoSettings.size = 1.0f;
        }

        updateProjectionMatrix();
    }

    glm::vec3 EditorCamera2D::calculatePosition() const {
        return focalPoint + glm::vec3(0.0f, 0.0f, orthoSettings.size);
    }

    std::pair<float, float> EditorCamera2D::getPanSpeed() const {
        float x = std::min(viewportWidth / 1000.0f, 2.4f);
        float y = std::min(viewportHeight / 1000.0f, 2.4f);

        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return {xFactor, yFactor};
    }

    float EditorCamera2D::getZoomSpeed() const {
        float dist = std::max(orthoSettings.size / 2, 0.0f);
        return std::min(dist * dist, 100.0f);
    }
};  // namespace AnEngine
