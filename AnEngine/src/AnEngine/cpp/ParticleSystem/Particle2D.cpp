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
    Particle2D::Particle2D() {
        props = {.position = glm::vec3(0.0f),
                 .velocity = glm::vec3(0.0f),
                 .startColour = glm::vec4(1.0f),
                 .endColour = glm::vec4(1.0f),
                 .rotation = 0.0f,
                 .rotationSpeed = 0.0f,
                 .startSize = 0.0f,
                 .endSize = 0.0f,
                 .lifeTime = 1.0f,
                 .lifeRemaining = 0.0f,
                 .alive = false};
        oldProps = props;
    }

    Particle2D::Particle2D(glm::vec3 position, glm::vec2 velocity,
                           std::pair<glm::vec4, glm::vec4> colourRange, float rotation,
                           float rotationSpeed, std::pair<float, float> sizeRange,
                           float lifeTime) {
        props = {.position = position,
                 .velocity = {velocity, 0.0f},
                 .startColour = colourRange.first,
                 .endColour = colourRange.second,
                 .rotation = rotation,
                 .rotationSpeed = rotationSpeed,
                 .startSize = sizeRange.first,
                 .endSize = sizeRange.second,
                 .lifeTime = lifeTime,
                 .lifeRemaining = lifeTime,
                 .alive = true};
        oldProps = props;
    }


    void Particle2D::reset() { props = oldProps; }

    void Particle2D::update(TimeStep delta) {
        props.lifeRemaining -= delta;
        props.position += props.velocity * (float)delta;
        props.rotation += props.rotationSpeed * delta;

        float life = props.lifeRemaining / props.lifeTime;
        props.colour = glm::lerp(props.endColour, props.startColour, life);
        props.colour.a *= life;

        props.size = glm::lerp(props.endSize, props.startSize, life);
    }

    void Particle2D::emit(float sizeVariation) {
        if (sizeVariation > 0.0f) {
            props.size += Random::getFloat() * sizeVariation - sizeVariation / 2.0f;
            // props.size += Random::getFloat(0.0f, sizeVariation);
        }

        Renderer2D::drawQuad(props.position, glm::vec2(props.size), props.rotation,
                             props.colour);
    }

    bool Particle2D::isAlive() const { return props.alive == true; }
    bool Particle2D::shouldDie() const { return props.lifeRemaining <= 0.0f; }

    void Particle2D::kill() { props.alive = false; }
}  // namespace AnEngine
