#ifndef PARTICLE2D_H
#define PARTICLE2D_H

#include <glm/glm.hpp>

#include "ParticleSystem/Particle.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class Particle2D : public Particle {
    public:
        virtual void init() override;
        virtual void reset() override;

        virtual void update(TimeStep delta) override;

        virtual void emit() override;

        virtual bool isAlive() const override;
        virtual bool shouldDie() const override;
        virtual void kill() override;


        friend class ParticleSpawner;

        struct {
            glm::vec3 velocity;
            glm::vec4 startColour;
            glm::vec4 endColour;
            float startRotation = 0.0f;
            float endRotation = 0.0f;
            float startSize;
            float endSize;
            float lifeTime;
        } props;

        float currentSize;

    private:
        bool initialised = false;

        struct {
            glm::vec3 velocity;
            glm::vec4 startColour;
            glm::vec4 endColour;
            float startRotation = 0.0f;
            float endRotation = 0.0f;
            float startSize;
            float endSize;
            float lifeTime;
        } masterProps;

        glm::vec4 currentColour;
        glm::vec3 currentPosition;
        float currentRotation;
        float lifeRemaining;
        bool alive;
    };
};  // namespace AnEngine

#endif
