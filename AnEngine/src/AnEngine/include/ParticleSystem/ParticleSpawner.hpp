#ifndef PARTICLESPAWNER_HPP
#define PARTICLESPAWNER_HPP

#include <vector>

#include "ParticleSystem/Particle.hpp"
#include "ParticleSystem/Particle2D.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class ParticleSpawner {
    public:
        ParticleSpawner() = default;
        ~ParticleSpawner() = default;

        void enable();
        void disable();
        void reset();

        void onUpdate(TimeStep deltaTime);
        void emit(Ref<Camera> camera);

        void setSizeVariation(float variation);
        void setSpawnRate(float rate);

        void addParticle(const Particle2D& particle);
        void addParticle(const Particle2D& particle, uint32_t count);
        void addParticles(const std::vector<Particle2D>& particles);

        void operator+=(const Particle2D& particle);
        void operator+=(const std::vector<Particle2D>& particles);

    private:
        bool enabled = false;
        float sizeVariation = 1.0f;
        float spawnRate = 0.0f;
        std::vector<Particle2D> particlePool;
        std::vector<Particle2D> masterParticlePool;
    };
}  // namespace AnEngine

#endif
