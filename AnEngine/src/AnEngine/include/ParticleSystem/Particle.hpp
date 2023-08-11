#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <glm/glm.hpp>

#include "Time/TimeStep.hpp"


namespace AnEngine {
    struct ParticleProperties {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec4 startColour;
        glm::vec4 endColour;
        glm::vec4 colour;
        float rotation = 0.f;
        float rotationSpeed = 0.0f;
        float startSize;
        float endSize;
        float size;
        float lifeTime = 1.0f;
        float lifeRemaining = 0.0f;

        bool alive = false;
    };

    class Particle {
    public:
        virtual ~Particle() = default;

        virtual void reset() = 0;

        virtual void update(TimeStep delta) = 0;

        virtual void emit(float sizeVariation) = 0;

        virtual bool isAlive() const = 0;
        virtual bool shouldDie() const = 0;
        virtual void kill() = 0;
    };
}  // namespace AnEngine

#endif
