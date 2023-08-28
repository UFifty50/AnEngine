#include "aepch.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include "Core/Random.hpp"
#include "ParticleSystem/Particle2D.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    void Particle2D::init() {
        if (initialised) return;
        initialised = true;

        memcpy(&this->masterProps, &this->props, sizeof(masterProps));
        currentColour = props.startColour;
        currentSize = props.startSize;
        currentRotation = props.startRotation;
        lifeRemaining = props.lifeTime;
        alive = true;
    }

    void Particle2D::reset() {
        if (!initialised) return;
        // initialised = false;

        std::memcpy(&this->props, &this->masterProps, sizeof(props));
        currentColour = props.startColour;
        currentSize = props.startSize;
        currentRotation = props.startRotation;
        lifeRemaining = props.lifeTime;
        alive = true;
    }

    void Particle2D::update(TimeStep delta) {
        if (!initialised) return;

        lifeRemaining -= delta;
        currentPosition += props.velocity * (float)delta;

        float life = lifeRemaining / props.lifeTime;
        currentColour = glm::lerp(props.endColour, props.startColour, life);
        currentColour.a *= life;

        currentSize = glm::lerp(props.endSize, props.startSize, life);
        currentRotation = glm::lerp(props.endRotation, props.startRotation, life);
    }

    void Particle2D::emit() {
        if (!initialised) return;

        Renderer2D::drawQuad(currentPosition, glm::vec2(currentSize), currentRotation,
                             currentColour);
    }

    bool Particle2D::isAlive() const { return alive == true; }
    bool Particle2D::shouldDie() const { return lifeRemaining <= 0.0f; }

    void Particle2D::kill() { alive = false; }
}  // namespace AnEngine
