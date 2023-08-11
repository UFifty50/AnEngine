#ifndef PARTICLE2D_H
#define PARTICLE2D_H

#include <glm/glm.hpp>

#include "ParticleSystem/Particle.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class Particle2D : public Particle {
    public:
        Particle2D();
        Particle2D(glm::vec3 position, glm::vec2 velocity,
                   std::pair<glm::vec4, glm::vec4> colourRange, float rotation,
                   float rotationSpeed, std::pair<float, float> sizeRange,
                   float lifeTime);

        virtual void reset() override;

        virtual void update(TimeStep delta) override;

        virtual void emit(float sizevariation) override;

        virtual bool isAlive() const override;
        virtual bool shouldDie() const override;
        virtual void kill() override;

    private:
        ParticleProperties props;
        ParticleProperties oldProps;
    };
};  // namespace AnEngine

#endif
