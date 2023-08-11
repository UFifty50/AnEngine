#include "aepch.hpp"

#include "ParticleSystem/ParticleSpawner.hpp"

#include <vector>

#include "Core/Random.hpp"
#include "ParticleSystem/Particle.hpp"
#include "ParticleSystem/Particle2D.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    void ParticleSpawner::enable() { enabled = true; }
    void ParticleSpawner::disable() { enabled = false; }

    void ParticleSpawner::reset() {
        for (auto& particle : masterParticlePool) {
            particle.reset();
        }

        particlePool = masterParticlePool;
    }

    void ParticleSpawner::onUpdate(TimeStep deltaTime) {
        if (!enabled) return;

        for (auto& particle : particlePool) {
            if (!particle.isAlive()) {
                continue;
            }

            if (particle.shouldDie()) {
                particle.kill();
                continue;
            }

            particle.update(deltaTime);
        }
    }

    void ParticleSpawner::emit(Ref<Camera> camera) {
        if (!enabled) return;

        Renderer2D::beginScene(camera);

        for (auto& particle : particlePool) {
            if (!particle.isAlive()) {
                continue;
            }

            particle.emit(sizeVariation);
        }

        Renderer2D::endScene();
    }

    void ParticleSpawner::setSizeVariation(float variation) { sizeVariation = variation; }

    void ParticleSpawner::setSpawnRate(float rate) {
        int repetitions = static_cast<int>(rate);
        float fraction = rate - repetitions;

        if (rate <= 0.0f) return;

        for (auto& value : masterParticlePool) {
            std::generate_n(std::back_inserter(particlePool), repetitions,
                            [&value]() { return value; });


            // Handle fractional part by random chance
            if (fraction > 0.0f && Random::getFloat() < fraction) {
                particlePool.push_back(value);
            }
        }

        // std::random_shuffle(particlePool.begin(), particlePool.end());
    }

    void ParticleSpawner::addParticle(const Particle2D& particle) {
        particlePool.push_back(particle);
        masterParticlePool.push_back(particle);
    }

    void ParticleSpawner::addParticle(const Particle2D& particle, uint32_t count) {
        for (uint32_t i = 0; i < count; i++) addParticle(particle);
    }

    void ParticleSpawner::addParticles(const std::vector<Particle2D>& particles) {
        particlePool.insert(particlePool.end(), particles.begin(), particles.end());
        masterParticlePool.insert(masterParticlePool.end(), particles.begin(),
                                  particles.end());
    }

    void ParticleSpawner::operator+=(const Particle2D& particle) {
        addParticle(particle);
    }

    void ParticleSpawner::operator+=(const std::vector<Particle2D>& particles) {
        addParticles(particles);
    }
}  // namespace AnEngine
