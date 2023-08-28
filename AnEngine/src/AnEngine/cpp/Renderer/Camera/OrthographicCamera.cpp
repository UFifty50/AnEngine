#include "aepch.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Camera/OrthographicCamera.hpp"


namespace AnEngine {
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom,
                                           float top)
        : projectionMaxtrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
          viewMatrix(1.0f),
          position(0.0f) {
        this->viewProjectionMatrix = this->projectionMaxtrix * this->viewMatrix;

        bounds.left = left;
        bounds.right = right;
        bounds.bottom = bottom;
        bounds.top = top;
    }

    void OrthographicCamera::recalculateViewMatrix() {
        glm::mat4 transfrom =
            glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));

        this->viewMatrix = glm::inverse(transfrom);
        this->viewProjectionMatrix = projectionMaxtrix * viewMatrix;
    }
};  // namespace AnEngine
