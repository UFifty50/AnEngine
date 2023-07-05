#ifndef TIMESTEP_HPP
#define TIMESTEP_HPP

namespace AnEngine {
    class TimeStep {
    public:
        TimeStep(float time = 0.0f) : time(time) {}

        operator float() const { return time; }
        float getSeconds() const { return time; }
        float getMilliseconds() const { return time * 1000.0f; }

    private:
        float time;
    };
}  // namespace AnEngine

#endif
