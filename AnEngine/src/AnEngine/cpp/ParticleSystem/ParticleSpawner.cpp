#include "aepch.hpp"

#include "ParticleSystem/ParticleSpawner.hpp"

#include <vector>

#include "Core/Input.hpp"
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

        activeParticles.clear();
    }

    void ParticleSpawner::onUpdate(TimeStep deltaTime) {
        if (!enabled) return;

        timeSinceLastAdd += deltaTime;
        int toAdd = static_cast<int>(timeSinceLastAdd * spawnRate);
        float fraction = timeSinceLastAdd * spawnRate - toAdd;
        timeSinceLastAdd -= toAdd / spawnRate;

        if (spawnRate > 0.0f && toAdd > 0) {
            for (int i = 0; i < toAdd; i++) {
                int index = Random::getInt(0, masterParticlePool.size() - 1);
                Particle2D value = masterParticlePool[index];

                if (sizeVariation > 0.0f) {
                    float variate = Random::getFloat(0.0f, sizeVariation);
                    //    Random::getFloat() * sizeVariation - sizeVariation / 2.0f;

                    value.props.startSize +=
                        Random::choice(std::array{variate / 2, -variate / 2});
                    value.props.endSize +=
                        Random::choice(std::array{variate / 2, -variate / 2});
                }
                value.currentPosition = position;

                activeParticles.push_back(value);
            }

            if (fraction > 0.0f && Random::getFloat() < fraction) {
                int index = Random::getInt(0, masterParticlePool.size() - 1);
                Particle2D value = masterParticlePool[index];

                if (sizeVariation > 0.0f) {
                    float variate = Random::getFloat(0.0f, sizeVariation);
                    //    Random::getFloat() * sizeVariation - sizeVariation / 2.0f;

                    value.props.startSize +=
                        Random::choice(std::array{variate / 2, -variate / 2});
                    value.props.endSize +=
                        Random::choice(std::array{variate / 2, -variate / 2});
                }
                value.currentPosition = position;

                activeParticles.push_back(value);
            }
        }

        for (auto& particle : activeParticles) {
            if (!particle.isAlive()) {
                continue;
            }

            if (particle.shouldDie()) {
                particle.kill();
                continue;
            }

            particle.update(deltaTime);

            activeParticles.erase(
                std::remove_if(activeParticles.begin(), activeParticles.end(),
                               [](const Particle2D& particle) {
                                   return !particle.isAlive() || particle.shouldDie();
                               }),
                activeParticles.end());
        }
    }

    void ParticleSpawner::emit(Ref<Camera> camera) {
        if (!enabled) return;

        Renderer2D::beginScene(camera);

        for (auto& particle : activeParticles) {
            if (!particle.isAlive()) {
                continue;
            }

            particle.emit();
        }

        Renderer2D::endScene();
    }

    void ParticleSpawner::setSizeVariation(float variation) { sizeVariation = variation; }

    void ParticleSpawner::setSpawnRate(float rate) { spawnRate = rate; }

    void ParticleSpawner::setPosition(glm::vec3 newPosition) { position = newPosition; }

    void ParticleSpawner::addParticle(Particle2D& particle) {
        particle.currentPosition = position;
        masterParticlePool.push_back(particle);
    }

    void ParticleSpawner::addParticle(Particle2D& particle, uint32_t count) {
        for (uint32_t i = 0; i < count; i++) {
            particle.currentPosition = position;
            addParticle(particle);
        }
    }

    void ParticleSpawner::addParticles(std::vector<Particle2D>& particles) {
        for (auto& particle : particles) {
            particle.currentPosition = position;
        }
        masterParticlePool.insert(masterParticlePool.end(), particles.begin(),
                                  particles.end());
    }

    void ParticleSpawner::operator+=(Particle2D& particle) { addParticle(particle); }

    void ParticleSpawner::operator+=(std::vector<Particle2D>& particles) {
        addParticles(particles);
    }
}  // namespace AnEngine
