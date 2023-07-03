#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include <glm/glm.hpp>


namespace AnEngine {
    class OrthographicCamera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        inline void setPosition(const glm::vec3& position) { this->position = position; recalculateViewMatrix(); }
        inline const glm::vec3& getPosition() const { return this->position; }

        inline void setRotation(float rotation) { this->rotation = rotation; recalculateViewMatrix(); }
        inline float getRotation() const { return this->rotation; }

        inline const glm::mat4& getProjectionMatrix() const { return this->projectionMaxtrix; }
        inline const glm::mat4& getViewMatrix() const { return this->viewMatrix; }
        inline const glm::mat4& getViewProjectionMatrix() const { return this->viewProjectionMatrix; }

    private:
        glm::mat4 projectionMaxtrix;
        glm::mat4 viewMatrix;
        glm::mat4 viewProjectionMatrix;

        glm::vec3 position;
        float rotation = 0.0f;

        void recalculateViewMatrix();
    };
};

#endif
