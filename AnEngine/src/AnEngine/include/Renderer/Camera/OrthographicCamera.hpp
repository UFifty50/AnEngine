#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Camera/Camera.hpp"

namespace AnEngine {
    class OrthographicCamera : public BaseCamera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);


        void setProjection(float left, float right, float bottom, float top) {
            projectionMaxtrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
            viewProjectionMatrix = projectionMaxtrix * viewMatrix;

            bounds.left = left;
            bounds.right = right;
            bounds.bottom = bottom;
            bounds.top = top;
        }

        virtual void setPosition(const glm::vec3& position) override {
            this->position = position;
            recalculateViewMatrix();
        }
        virtual const glm::vec3& getPosition() const override { return this->position; }

        virtual void setRotation(float rotation) override {
            this->rotation = rotation;
            recalculateViewMatrix();
        }
        virtual float getRotation() const override { return this->rotation; }

        virtual const glm::mat4& getProjectionMatrix() const override {
            return this->projectionMaxtrix;
        }

        virtual const glm::mat4& getViewMatrix() const override {
            return this->viewMatrix;
        }

        virtual const glm::mat4& getViewProjectionMatrix() const override {
            return this->viewProjectionMatrix;
        }

        virtual CameraType getType() const override { return CameraType::Orthographic; }

        decltype(auto) getBounds() const { return bounds; }

    private:
        glm::mat4 projectionMaxtrix;
        glm::mat4 viewMatrix;
        glm::mat4 viewProjectionMatrix;

        glm::vec3 position;
        float rotation = 0.0f;

        struct {
            float left, right;
            float bottom, top;

            float getWidth() const { return right - left; }
            float getHeight() const { return top - bottom; }
        } bounds;

        void recalculateViewMatrix();
    };
};  // namespace AnEngine

#endif
