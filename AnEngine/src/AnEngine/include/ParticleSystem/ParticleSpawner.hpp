#ifndef PARTICLESPAWNER_HPP
#define PARTICLESPAWNER_HPP

#include <glm/glm.hpp>

#include <vector>

#include "ParticleSystem/Particle.hpp"
#include "ParticleSystem/Particle2D.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class ParticleSpawner {
    public:
        ParticleSpawner(glm::vec3 position) : position(position) {}

        void enable();
        void disable();
        void reset();

        void onUpdate(TimeStep deltaTime);
        void emit(Ref<Camera> camera);

        void setSizeVariation(float variation);
        void setSpawnRate(float rate);
        void setPosition(glm::vec3 newPosition);

        void addParticle(Particle2D& particle);
        void addParticle(Particle2D& particle, uint32_t count);
        void addParticles(std::vector<Particle2D>& particles);

        void operator+=(Particle2D& particle);
        void operator+=(std::vector<Particle2D>& particles);

    private:
        void addToActive(int count);


        bool enabled = false;
        float spawnRate = 1.0f;
        float timeSinceLastAdd = 0.0f;
        float sizeVariation = 0.0f;

        glm::vec3 position;

        std::vector<Particle2D> activeParticles;
        std::vector<Particle2D> masterParticlePool;
    };
}  // namespace AnEngine

#endif
