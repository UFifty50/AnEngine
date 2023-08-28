#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <glm/glm.hpp>

#include "Time/TimeStep.hpp"


namespace AnEngine {
    class Particle {
    public:
        virtual ~Particle() = default;

        virtual void init() = 0;
        virtual void reset() = 0;

        virtual void update(TimeStep delta) = 0;

        virtual void emit() = 0;

        virtual bool isAlive() const = 0;
        virtual bool shouldDie() const = 0;
        virtual void kill() = 0;
    };
}  // namespace AnEngine

#endif
